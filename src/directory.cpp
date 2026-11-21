#include "directory.h"
#include <stdexcept>
#include <algorithm>
Directory::Directory(const std::string& name, Directory* parent)
    : name(name), parent(parent) {}

const std::string& Directory::getName() const {
    return name;
}

Directory* Directory::getParent() const {
    return parent;
}

bool Directory::hasFile(const std::string& n) const {
    return files.count(n) > 0;
}

bool Directory::hasSubdir(const std::string& n) const {
    return subdirs.count(n) > 0;
}

File* Directory::getFile(const std::string& n) const {
    auto it = files.find(n);
    if (it == files.end()) {
        return nullptr;
    }
    return it->second.get();
}

Directory* Directory::getSubdir(const std::string& n) const {
    auto it = subdirs.find(n);
    if (it == subdirs.end()) {
        return nullptr;
    }
    return it->second.get();
}

File* Directory::createFile(const std::string& n) {
    if (hasFile(n) || hasSubdir(n)) {
        throw std::runtime_error("Name already exists: " + n);
    }
    files[n] = std::make_unique<File>(n);
    return files[n].get();
}

Directory* Directory::createSubdir(const std::string& n) {
    if (hasFile(n) || hasSubdir(n)) {
        throw std::runtime_error("Name already exists: " + n);
    }
    subdirs[n] = std::make_unique<Directory>(n, this);
    return subdirs[n].get();
}

void Directory::removeFile(const std::string& n) {
    if (!hasFile(n)) {
        throw std::runtime_error("File not found: " + n);
    }
    files.erase(n);
}

std::string Directory::getPath() const {
    if (parent == nullptr) {
        return "/";
    }
    std::string parentPath = parent->getPath();
    if (parentPath == "/") {
        return "/" + name;
    }
    return parentPath + "/" + name;
}

std::vector<std::string> Directory::list() const {
    std::vector<std::string> result;
    for (const auto& pair : files) {
        result.push_back(pair.first);
    }
    for (const auto& pair : subdirs) {
        result.push_back(pair.first);
    }
    std::sort(result.begin(), result.end());
    return result;
}