#pragma once
#include <string>
#include <vector>

class Command {
public:
    Command(const std::string& command);

    std::vector<std::string> arguments;
    std::string mainCommand;
    std::string fullCommand;
};