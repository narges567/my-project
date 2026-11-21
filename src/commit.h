#pragma once
#include <map>
#include <string>
#include <vector>

using Snapshot = std::map<std::string, std::vector<std::string>>;

class Commit {
private:
    int id;
    std::vector<int> parents;
    std::string message;
    std::string authorName;
    std::string authorEmail;
    std::string timestamp;
    Snapshot snapshot;

public:
    Commit(int id, const std::vector<int>& parents, const std::string& message,
           const std::string& authorName, const std::string& authorEmail,
           const std::string& timestamp, const Snapshot& snapshot);

    int getId() const;
    const std::vector<int>& getParents() const;
    const std::string& getMessage() const;
    const std::string& getAuthorName() const;
    const std::string& getAuthorEmail() const;
    const std::string& getTimestamp() const;
    const Snapshot& getSnapshot() const;
};