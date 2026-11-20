#include <iostream>
#include "File.h"

int main() {
    File f("a.txt");
    f.overwrite("hello");
    f.append("world");
    for (const auto& line : f.getLines()) {
        std::cout << line << std::endl;
    }
    return 0;
}