#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "file.h"

class Directory {
private:
    std::string name;
    Directory* parent;
    std::map<std::string, std::unique_ptr<File>> files;
    std::map<std::string, std::unique_ptr<Directory>> subdirs;

public:
    Directory(const std::string& name, Directory* parent);

    const std::string& getName() const;
    Directory* getParent() const;

    bool hasFile(const std::string& name) const;
    bool hasSubdir(const std::string& name) const;

    File* getFile(const std::string& name) const;
    Directory* getSubdir(const std::string& name) const;

    File* createFile(const std::string& name);
    Directory* createSubdir(const std::string& name);
    void removeFile(const std::string& name);

    std::string getPath() const;
    std::vector<std::string> list() const;
};