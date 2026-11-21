#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "commit.h"
#include "directory.h"
#include <stdexcept>
#include "user.h"
#include <ctime>

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

    std::string currentTime() const;

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

    std::string commit(const std::string& message, const User& user);

    std::string status() const;
    std::string log() const;
};