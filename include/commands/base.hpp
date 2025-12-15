#pragma once

#include <core/command.hpp>
#include <unordered_map>
#include <string>
#include <functional>

namespace Commands {
    int echo(Command command);
    int cd(Command command);
}

struct Builtin {
    std::function<int(Command)> func;
    std::string description;
};

static std::unordered_map<std::string, Builtin> map = {
    {"echo", {Commands::echo, "Prints text to the screen."}},
    {"cd", {Commands::cd, "Change current directory."}},
};