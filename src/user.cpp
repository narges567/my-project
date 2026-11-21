#include "user.h"

void User::setName(const std::string& n) { name = n; }
void User::setEmail(const std::string& e) { email = e; }
const std::string& User::getName() const { return name; }
const std::string& User::getEmail() const { return email; }

bool User::isConfigured() const {
    return !name.empty() && !email.empty();
}