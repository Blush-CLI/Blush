/*
 File: src/commands/help.cpp
 Created: [25.08.2025]
 Command: help
 Description: iterates through all of these and displays each available built-in command
 Part-of: built-in
*/

#include <iostream>
#include <vector>
#include "../includes/cmd.h"
#include "../includes/bcolors.h"

void c_help(const std::vector<std::string>& args) {
    setColor(chosenColor);
    std::cout << "\nBlush Built-in Commands:\n";
    setColor();
    for (const auto& [name, func] : commands) {
        setColor(Color::Blue);
        std::cout << "  > ";
        setColor(chosenColor);
        std::cout << name << "\n";
        setColor();
    }
}
