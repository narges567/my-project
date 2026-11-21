#include <iostream>
#include "commit.h"

int main() {
    Snapshot s;
    s["a.txt"] = {"hello", "world"};

    Commit c1(1, {}, "first commit", "ali", "ali@example.com", "2026-11-06 10:25:12", s);
    Commit c2(2, {1}, "add file", "ali", "ali@example.com", "2026-11-06 10:30:00", s);

    std::cout << "commit " << c2.getId() << std::endl;
    std::cout << "Parent: " << c2.getParents()[0] << std::endl;
    std::cout << "Author: " << c2.getAuthorName() << " <" << c2.getAuthorEmail() << ">" << std::endl;
    std::cout << "Lines in a.txt: " << c2.getSnapshot().at("a.txt").size() << std::endl;
    std::cout << "Root parents: " << c1.getParents().size() << std::endl;
    return 0;
}