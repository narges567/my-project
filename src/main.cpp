#include <iostream>
#include "user.h"
#include "repository.h"
#include "filesystem.h"

int main() {
    User u;
    std::cout << "Configured: " << u.isConfigured() << std::endl;
    u.setName("ali");
    u.setEmail("ali@example.com");
    std::cout << "Configured: " << u.isConfigured() << std::endl;

    FileSystem fs;
    Repository repo(fs.getRoot());
    std::cout << "Branch: " << repo.getCurrentBranch() << std::endl;
    std::cout << "Detached: " << repo.isDetached() << std::endl;
    std::cout << "Commits: " << repo.getCommitCount() << std::endl;
    return 0;
}