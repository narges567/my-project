#include <iostream>
#include "repository.h"
#include "filesystem.h"
#include "user.h"

int main() {
    FileSystem fs;
    Repository repo(fs.getRoot());
    User user;
    try {
        fs.echo("hello", "a.txt", false);

        try {
            repo.commit("first", user);
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }

        user.setName("ali");
        user.setEmail("ali@example.com");

        std::cout << repo.commit("first", user) << std::endl;

        repo.add("a.txt", fs.getCurrent());
        std::cout << repo.commit("first commit", user) << std::endl;

        fs.echo("world", "a.txt", true);
        repo.add("a.txt", fs.getCurrent());
        std::cout << repo.commit("add second line", user) << std::endl;

        std::cout << repo.commit("again", user) << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    return 0;
}