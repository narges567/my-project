#include "commit.h"

Commit::Commit(int id, const std::vector<int>& parents, const std::string& message,
               const std::string& authorName, const std::string& authorEmail,
               const std::string& timestamp, const Snapshot& snapshot)
    : id(id), parents(parents), message(message), authorName(authorName),
      authorEmail(authorEmail), timestamp(timestamp), snapshot(snapshot) {}

int Commit::getId() const { return id; }
const std::vector<int>& Commit::getParents() const { return parents; }
const std::string& Commit::getMessage() const { return message; }
const std::string& Commit::getAuthorName() const { return authorName; }
const std::string& Commit::getAuthorEmail() const { return authorEmail; }
const std::string& Commit::getTimestamp() const { return timestamp; }
const Snapshot& Commit::getSnapshot() const { return snapshot; }