#include <iostream>
#include "filesystem.h"

int main() {
    FileSystem fs;
    try {
        fs.mkdir("docs");
        fs.cd("docs");
        fs.echo("hello", "a.txt", false);
        fs.echo("world", "a.txt", true);
        std::cout << fs.pwd() << std::endl;
        for (const auto& line : fs.cat("a.txt")) {
            std::cout << line << std::endl;
        }
        fs.cd("..");
        for (const auto& name : fs.ls()) {
            std::cout << name << std::endl;
        }
        fs.cd("nothing");
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    return 0;
}