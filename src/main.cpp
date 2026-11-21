#include <iostream>
#include "directory.h"

int main() {
    Directory root("", nullptr);

    Directory* docs = root.createSubdir("docs");
    docs->createFile("a.txt")->overwrite("hello");
    root.createFile("b.txt");

    std::cout << "Path: " << docs->getPath() << std::endl;
    for (const auto& name : root.list()) {
        std::cout << name << std::endl;
    }

    try {
        root.createSubdir("docs");
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    return 0;
}