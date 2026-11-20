#include <iostream>
#include "FileSystem.h"

int main() {
    FileSystem::createDirectory("test_folder");

    FileSystem::writeFile("test_folder/hello.txt", "Hello, Git Simulator!");

    if (FileSystem::exists("test_folder/hello.txt")) {
        std::cout << "File exists." << std::endl;
        std::cout << "Content: " << FileSystem::readFile("test_folder/hello.txt") << std::endl;
    } else {
        std::cout << "File not found." << std::endl;
    }

    return 0;
}