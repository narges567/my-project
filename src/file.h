#pragma once
#include <string>
#include <vector>

class File {
private:
    std::string name;
    std::vector<std::string> lines;

public:
    explicit File(const std::string& name);

    const std::string& getName() const;
    const std::vector<std::string>& getLines() const;

    void overwrite(const std::string& text);
    void append(const std::string& text);
};