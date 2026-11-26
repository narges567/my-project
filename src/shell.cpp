#include "shell.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

std::vector<std::string> Shell::tokenize(const std::string& line) const {
    std::vector<std::string> tokens;
    std::string cur;
    bool inQuotes = false;
    bool hasToken = false;
    for (char ch : line) {
        if (ch == '"') {
            inQuotes = !inQuotes;
            hasToken = true;
        } else if (ch == ' ' && !inQuotes) {
            if (hasToken) {
                tokens.push_back(cur);
                cur.clear();
                hasToken = false;
            }
        } else {
            cur += ch;
            hasToken = true;
        }
    }
    if (inQuotes) {
        throw std::runtime_error("Unterminated quote");
    }
    if (hasToken) {
        tokens.push_back(cur);
    }
    return tokens;
}

void Shell::handleEcho(const std::string& line) {
    std::vector<std::string> t = tokenize(line);
    if (t.size() != 4 || (t[2] != ">" && t[2] != ">>")) {
        throw std::runtime_error("Usage: echo \"<TEXT>\" > <FILE_NAME>");
    }
    fs.echo(t[1], t[3], t[2] == ">>");
}

void Shell::executeGit(const std::vector<std::string>& a) {
    if (a.size() < 2) {
        throw std::runtime_error("Usage: git <command>");
    }
    const std::string& cmd = a[1];

    if (cmd == "config") {
        if (a.size() != 4) {
            throw std::runtime_error("Usage: git config user.name|user.email \"<VALUE>\"");
        }
        if (a[2] == "user.name") {
            user.setName(a[3]);
        } else if (a[2] == "user.email") {
            user.setEmail(a[3]);
        } else {
            throw std::runtime_error("Unknown config key: " + a[2]);
        }
        return;
    }

    if (cmd == "init") {
        if (repo != nullptr) {
            throw std::runtime_error("Repository already exists");
        }
        repo = std::make_unique<Repository>(fs.getCurrent());
        std::cout << "Initialized empty repository" << std::endl;
        return;
    }

    if (repo == nullptr) {
        throw std::runtime_error("Not a git repository");
    }

    if (cmd == "add") {
        if (a.size() != 3) {
            throw std::runtime_error("Usage: git add <FILE_NAME>|.");
        }
        if (a[2] == ".") {
            repo->addAll(fs.getCurrent());
        } else {
            repo->add(a[2], fs.getCurrent());
        }
    } else if (cmd == "reset") {
        if (a.size() != 3) {
            throw std::runtime_error("Usage: git reset <FILE_NAME>");
        }
        repo->reset(a[2], fs.getCurrent());
    } else if (cmd == "commit") {
        if (a.size() != 4 || a[2] != "-m") {
            throw std::runtime_error("Usage: git commit -m \"<MESSAGE>\"");
        }
        std::cout << repo->commit(a[3], user) << std::endl;
    } else if (cmd == "status") {
        std::cout << repo->status();
    } else if (cmd == "log") {
        std::cout << repo->log();
    } else if (cmd == "branch") {
      if (a.size() != 2) {
        throw std::runtime_error("Usage: git branch");
    }
    std::cout << repo->branchList();

    } else {
        throw std::runtime_error("Unknown git command: "+ cmd);
    }

}

void Shell::execute(const std::string& line) {
    std::vector<std::string> a = tokenize(line);
    if (a.empty()) {
        return;
    }
    const std::string& cmd = a[0];

    if (cmd == "echo") {
        handleEcho(line);
    } else if (cmd == "pwd") {
        std::cout << fs.pwd() << std::endl;
    } else if (cmd == "mkdir") {
        if (a.size() != 2) throw std::runtime_error("Usage: mkdir <DIRECTORY_NAME>");
        fs.mkdir(a[1]);
    } else if (cmd == "cd") {
        if (a.size() != 2) throw std::runtime_error("Usage: cd <DIRECTORY_ADDRESS>");
        fs.cd(a[1]);
    } else if (cmd == "ls") {
        for (const auto& name : fs.ls()) {
            std::cout << name << std::endl;
        }
    } else if (cmd == "touch") {
        if (a.size() != 2) throw std::runtime_error("Usage: touch <FILE_NAME>");
        fs.touch(a[1]);
    } else if (cmd == "cat") {
        if (a.size() != 2) throw std::runtime_error("Usage: cat <FILE_NAME>");
        for (const auto& l : fs.cat(a[1])) {
            std::cout << l << std::endl;
        }
    } else if (cmd == "rm") {
        if (a.size() != 2) throw std::runtime_error("Usage: rm <FILE_NAME>");
        fs.rm(a[1]);
    } else if (cmd == "git") {
        executeGit(a);
    } else {
        throw std::runtime_error("Unknown command: " + cmd);
    }
}

void Shell::run() {
    std::string line;
    while (true) {
        std::cout << "$ ";
        if (!std::getline(std::cin, line)) {
            break;
        }
        if (line == "exit") {
            break;
        }
        try {
            execute(line);
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }
}