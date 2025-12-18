#pragma once

#include <core/command.hpp>
#include <unordered_map>
#include <string>
#include <functional>

namespace Commands {
    int echo(Command command);
    int cd(Command command);
    int clear(Command command);
    int exit(Command command);
    int help(Command command);
    int history(Command command);
    int play(Command command);
}

struct Builtin {
    std::function<int(Command)> func;
    std::string description;
};

static std::unordered_map<std::string, Builtin> commands_map = {
    {"echo", {Commands::echo, "Prints text to the screen."}},
    {"cd", {Commands::cd, "Change current directory."}},
    {"clear", {Commands::clear, "Clears the screen."}},
    {"cls", {Commands::clear, "Clears the screen."}},
    {"q", {Commands::exit, "Exists Blush."}},
    {"help", {Commands::help, "Displays help instructions."}},
    {"history", {Commands::history, "Displays command history."}},
    {"play-music", {Commands::play, "Plays music."}},
};