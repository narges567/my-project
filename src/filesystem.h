#pragma once
#include <memory>
#include <string>
#include <vector>
#include "directory.h"

class FileSystem {
private:
    std::unique_ptr<Directory> root;
    Directory* current;

    Directory* resolvePath(const std::string& path) const;

public:
    FileSystem();

    Directory* getCurrent() const;
    Directory* getRoot() const;

    std::string pwd() const;
    void mkdir(const std::string& name);
    void cd(const std::string& path);
    std::vector<std::string> ls() const;
    void touch(const std::string& name);
    void echo(const std::string& text, const std::string& fileName, bool append);
    std::vector<std::string> cat(const std::string& fileName) const;
    void rm(const std::string& fileName);
};