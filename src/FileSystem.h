#pragma once
#include <string>
class FileSystem {
    public:
    static bool exists (const std::string& path);
    static std::string readFile(const std::string& path);
    static bool writeFile (const std::string& path, const std::string& content);
    static bool createDirectory (const std::string& path);
};