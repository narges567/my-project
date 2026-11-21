#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "commit.h"
#include "directory.h"
#include <stdexcept>

class Repository {
private:
    Directory* rootDir;                
    std::vector<std::unique_ptr<Commit>> commits;
    std::map<std::string, int> branches; 
    std::string currentBranch;          
    int headCommit;                    
    Snapshot staging;

    std::string relativePath(Directory* dir, const std::string& fileName) const;
    void collectFiles(Directory* dir, Snapshot& out) const;
    const Snapshot& lastCommitSnapshot() const;

public:
    explicit Repository(Directory* rootDir);

    Directory* getRootDir() const;
    const std::string& getCurrentBranch() const;
    bool isDetached() const;
    int getHeadCommit() const;
    int getCommitCount() const;

    void add(const std::string& fileName, Directory* currentDir);
    void addAll(Directory* currentDir);
    void reset(const std::string& fileName, Directory* currentDir);
    const Snapshot& getStaging() const;
};