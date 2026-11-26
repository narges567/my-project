#include "repository.h"
#include <stdexcept>
#include <ctime>

Repository::Repository(Directory* rootDir)
    : rootDir(rootDir), currentBranch("main"), headCommit(0) {
    branches["main"] = 0;
}

Directory* Repository::getRootDir() const { return rootDir; }
const std::string& Repository::getCurrentBranch() const { return currentBranch; }
bool Repository::isDetached() const { return currentBranch.empty(); }
int Repository::getHeadCommit() const { return headCommit; }
int Repository::getCommitCount() const { return static_cast<int>(commits.size()); }
const Snapshot& Repository::getStaging() const {
    return staging;
}

const Snapshot& Repository::lastCommitSnapshot() const {
    static const Snapshot empty;
    if (headCommit == 0) {
        return empty;
    }
    return commits[headCommit - 1]->getSnapshot();
}

std::string Repository::relativePath(Directory* dir, const std::string& fileName) const {
    std::string full = dir->getPath();
    std::string rootPath = rootDir->getPath();
    std::string rel;
    if (full.size() > rootPath.size()) {
        rel = full.substr(rootPath.size());
        if (rel[0] == '/') {
            rel = rel.substr(1);
        }
    }
    if (rel.empty()) {
        return fileName;
    }
    return rel + "/" + fileName;
}

void Repository::collectFiles(Directory* dir, Snapshot& out) const {
    for (const auto& name : dir->list()) {
        File* f = dir->getFile(name);
        if (f != nullptr) {
            out[relativePath(dir, name)] = f->getLines();
        } else {
            collectFiles(dir->getSubdir(name), out);
        }
    }
}

void Repository::add(const std::string& fileName, Directory* currentDir) {
    std::string path = relativePath(currentDir, fileName);
    File* f = currentDir->getFile(fileName);
    if (f != nullptr) {
        staging[path] = f->getLines();
    } else if (staging.count(path) > 0) {
        staging.erase(path);  
    } else {
        throw std::runtime_error("pathspec '" + fileName + "' did not match any files");
    }
}

void Repository::addAll(Directory* currentDir) {
    Snapshot working;
    collectFiles(currentDir, working);

    std::string prefix = relativePath(currentDir, "");

    for (const auto& pair : working) {
        staging[pair.first] = pair.second;
    }
    std::vector<std::string> toErase;
    for (const auto& pair : staging) {
        bool inScope = prefix.empty() || pair.first.rfind(prefix, 0) == 0;
        if (inScope && working.count(pair.first) == 0) {
            toErase.push_back(pair.first);
        }
    }
    for (const auto& p : toErase) {
        staging.erase(p);
    }
}

void Repository::reset(const std::string& fileName, Directory* currentDir) {
    std::string path = relativePath(currentDir, fileName);
    const Snapshot& last = lastCommitSnapshot();
    auto it = last.find(path);
    if (it != last.end()) {
        staging[path] = it->second;
    } else {
        staging.erase(path);
    }

    
}

std::string Repository::currentTime() const {
    std::time_t t = std::time(nullptr);
    char buf[32];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
    return std::string(buf);
}

std::string Repository::commit(const std::string& message, const User& user) {
    if (!user.isConfigured()) {
        throw std::runtime_error("Please configure user.name and user.email first");
    }
    if (isDetached()) {
        throw std::runtime_error("Cannot commit in detached HEAD state");
    }

    const Snapshot& last = lastCommitSnapshot();
    if (staging == last) {
        Snapshot working;
        collectFiles(rootDir, working);
        if (working == staging) {
            return "nothing to commit, working tree clean";
        }
        return "no changes added to commit";
    }

    int newId = static_cast<int>(commits.size()) + 1;
    std::vector<int> parents;
    if (headCommit != 0) {
        parents.push_back(headCommit);
    }
    commits.push_back(std::make_unique<Commit>(
        newId, parents, message, user.getName(), user.getEmail(),
        currentTime(), staging));

    headCommit = newId;
    branches[currentBranch] = newId;

    return "[" + currentBranch + " " + std::to_string(newId) + "] " + message;
}
std::string Repository::status() const {
    std::string out;
    if (isDetached()) {
        out += "HEAD detached at " + std::to_string(headCommit) + "\n";
    } else {
        out += "On branch " + currentBranch + "\n";
    }

    const Snapshot& last = lastCommitSnapshot();
    Snapshot working;
    collectFiles(rootDir, working);

    std::string staged;
    for (const auto& pair : staging) {
        auto it = last.find(pair.first);
        if (it == last.end()) {
            staged += "    new file: " + pair.first + "\n";
        } else if (it->second != pair.second) {
            staged += "    modified: " + pair.first + "\n";
        }
    }
    for (const auto& pair : last) {
        if (staging.count(pair.first) == 0) {
            staged += "    deleted: " + pair.first + "\n";
        }
    }

    std::string modified;
    for (const auto& pair : staging) {
        auto it = working.find(pair.first);
        if (it == working.end()) {
            modified += "    deleted: " + pair.first + "\n";
        } else if (it->second != pair.second) {
            modified += "    modified: " + pair.first + "\n";
        }
    }

    // بخش ۴: Untracked files
    std::string untracked;
    for (const auto& pair : working) {
        if (staging.count(pair.first) == 0) {
            untracked += "    " + pair.first + "\n";
        }
    }

    if (staged.empty() && modified.empty() && untracked.empty()) {
        out += "nothing to commit, working tree clean\n";
        return out;
    }
    if (!staged.empty()) {
        out += "Staged changes:\n" + staged;
    }
    if (!modified.empty()) {
        out += "Modified (not staged):\n" + modified;
    }
    if (!untracked.empty()) {
        out += "Untracked files:\n" + untracked;
    }
    return out;
}

std::string Repository::log() const {
    if (headCommit == 0) {
        throw std::runtime_error("No commits yet");
    }
    std::string out;
    int id = headCommit;
    bool first = true;
    while (id != 0) {
        const Commit& c = *commits[id - 1];
        if (!first) {
            out += "\n";
        }
        first = false;
        out += "commit " + std::to_string(c.getId()) + "\n";
        out += "Author: " + c.getAuthorName() + " <" + c.getAuthorEmail() + ">\n";
        out += "Date: " + c.getTimestamp() + "\n";
        out += "    " + c.getMessage() + "\n";
        id = c.getParents().empty() ? 0 : c.getParents()[0];
    }
    return out;
}

std::string Repository::branchList() const {
    std::string out;
    for (const auto& pair : branches) {
        if (pair.first == currentBranch) {
            out += "* ";
        } else {
            out += "  ";
        }
        out += pair.first + "\n";
    }
    return out;
}