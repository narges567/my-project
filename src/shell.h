#pragma once
#include <memory>
#include <string>
#include <vector>
#include "filesystem.h"
#include "repository.h"
#include "user.h"

class Shell {
private:
    FileSystem fs;
    User user;
    std::unique_ptr<Repository> repo;   
    std::vector<std::string> tokenize(const std::string& line) const;
    void execute(const std::string& line);
    void executeGit(const std::vector<std::string>& args);
    void handleEcho(const std::string& line);

public:
    void run();
};