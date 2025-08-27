// includes/bbcmds.h - Basic Built-in Commands
// Blush - 1.0.0

#include <iostream>
#include <string>
#include <map>
#include <functional>

void helpCommand();
void aboutCommand();
void versionCommand();
void authorCommand();

std::map<std::string, std::function<void()>> bbcommands = {
    {"help", helpCommand},
    {"cmds", helpCommand},
    {"about", aboutCommand},
    {"version", versionCommand},
    {"author", authorCommand}
};