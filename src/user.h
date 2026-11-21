#pragma once
#include <string>

class User {
private:
    std::string name;
    std::string email;

public:
    void setName(const std::string& n);
    void setEmail(const std::string& e);
    const std::string& getName() const;
    const std::string& getEmail() const;
    bool isConfigured() const;
};