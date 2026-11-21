#include "file.h"

File::File(const std::string& name) : name(name) {}

const std::string& File::getName() const {
    return name;
}

const std::vector<std::string>& File::getLines() const {
    return lines;
}

void File::overwrite(const std::string& text) {
    lines.clear();
    lines.push_back(text);
}

void File::append(const std::string& text) {
    lines.push_back(text);
}