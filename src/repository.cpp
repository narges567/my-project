#include "repository.h"
#include <stdexcept>

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