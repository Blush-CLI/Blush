#pragma once
#include <string>
#include <vector>

class History {
public:
    void add(const std::string& command);
    std::string getPrevious();
    std::string getNext();
    void reset();
    bool isEmpty() const;
    size_t size() const;
    void clear();
    const std::vector<std::string>& getCommands() const;

private:
    std::vector<std::string> commands;
    int currentIndex = -1;
};