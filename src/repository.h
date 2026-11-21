#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "commit.h"
#include "directory.h"

class Repository {
private:
    Directory* rootDir;                
    std::vector<std::unique_ptr<Commit>> commits;
    std::map<std::string, int> branches; 
    std::string currentBranch;          
    int headCommit;                    
    Snapshot staging;

public:
    explicit Repository(Directory* rootDir);

    Directory* getRootDir() const;
    const std::string& getCurrentBranch() const;
    bool isDetached() const;
    int getHeadCommit() const;
    int getCommitCount() const;
};