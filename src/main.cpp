#include <iostream>
#include "repository.h"
#include "filesystem.h"
#include "user.h"

int main() {
    FileSystem fs;
    Repository repo(fs.getRoot());
    User user;
    user.setName("ali");
    user.setEmail("ali@example.com");
    try {
        std::cout << repo.status() << std::endl;

        fs.echo("hello", "a.txt", false);
        fs.echo("x", "b.txt", false);
        std::cout << repo.status() << std::endl;

        repo.add("a.txt", fs.getCurrent());
        std::cout << repo.status() << std::endl;

        repo.commit("first commit", user);
        fs.echo("world", "a.txt", true);
        std::cout << repo.status() << std::endl;

        repo.add("a.txt", fs.getCurrent());
        repo.commit("add second line", user);
        std::cout << repo.log() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    return 0;
}