#include "filesystem.h"
#include <sstream>
#include <stdexcept>

FileSystem::FileSystem()
    : root(std::make_unique<Directory>("", nullptr)), current(nullptr) {
    current = root.get();
}

Directory* FileSystem::getCurrent() const {
    return current;
}

Directory* FileSystem::getRoot() const {
    return root.get();
}

Directory* FileSystem::resolvePath(const std::string& path) const {
    Directory* dir = current;
    std::string part;
    std::string p = path;

    if (!p.empty() && p[0] == '/') {
        dir = root.get();
    }

    std::stringstream ss(p);
    while (std::getline(ss, part, '/')) {
        if (part.empty() || part == ".") {
            continue;
        }
        if (part == "..") {
            if (dir->getParent() != nullptr) {
                dir = dir->getParent();
            }
            continue;
        }
        Directory* next = dir->getSubdir(part);
        if (next == nullptr) {
            throw std::runtime_error("No such directory: " + path);
        }
        dir = next;
    }
    return dir;
}

std::string FileSystem::pwd() const {
    return current->getPath();
}

void FileSystem::mkdir(const std::string& name) {
    current->createSubdir(name);
}

void FileSystem::cd(const std::string& path) {
    current = resolvePath(path);
}

std::vector<std::string> FileSystem::ls() const {
    return current->list();
}

void FileSystem::touch(const std::string& name) {
    if (current->hasSubdir(name)) {
        throw std::runtime_error("A directory with this name exists: " + name);
    }
    if (!current->hasFile(name)) {
        current->createFile(name);
    }
}

void FileSystem::echo(const std::string& text, const std::string& fileName, bool append) {
    if (current->hasSubdir(fileName)) {
        throw std::runtime_error("Not a file: " + fileName);
    }
    File* f = current->getFile(fileName);
    if (f == nullptr) {
        f = current->createFile(fileName);
    }
    if (append) {
        f->append(text);
    } else {
        f->overwrite(text);
    }
}

std::vector<std::string> FileSystem::cat(const std::string& fileName) const {
    File* f = current->getFile(fileName);
    if (f == nullptr) {
        throw std::runtime_error("File not found: " + fileName);
    }
    return f->getLines();
}

void FileSystem::rm(const std::string& fileName) {
    if (current->hasSubdir(fileName)) {
        throw std::runtime_error("Is a directory: " + fileName);
    }
    current->removeFile(fileName);
}