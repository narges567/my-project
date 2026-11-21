#include <iostream>
#include "repository.h"
#include "filesystem.h"

int main() {
    FileSystem fs;
    Repository repo(fs.getRoot());
    try {
        fs.echo("hello", "a.txt", false);
        fs.mkdir("docs");
        fs.cd("docs");
        fs.echo("x", "b.txt", false);
        fs.cd("/");

        repo.add("a.txt", fs.getCurrent());
        std::cout << "Staged: " << repo.getStaging().size() << std::endl;

        repo.addAll(fs.getCurrent());
        std::cout << "Staged after add .: " << repo.getStaging().size() << std::endl;

        repo.reset("a.txt", fs.getCurrent());
        std::cout << "Staged after reset: " << repo.getStaging().size() << std::endl;

        repo.add("nothing.txt", fs.getCurrent());
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    return 0;
}