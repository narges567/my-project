#include "repository.h"

Repository::Repository(Directory* rootDir)
    : rootDir(rootDir), currentBranch("main"), headCommit(0) {
    branches["main"] = 0;
}

Directory* Repository::getRootDir() const { return rootDir; }
const std::string& Repository::getCurrentBranch() const { return currentBranch; }
bool Repository::isDetached() const { return currentBranch.empty(); }
int Repository::getHeadCommit() const { return headCommit; }
int Repository::getCommitCount() const { return static_cast<int>(commits.size()); }