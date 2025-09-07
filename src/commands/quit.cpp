/*
 File: src/commands/quit.cpp
 Created: [25.08.2025]
 Command: quit
 Description: Exites the program
 Part-of: built-in
*/

#include <iostream>
#include "../includes/bcolors.h"

void c_quit(const std::vector<std::string>& args = {}) {
    setColor();
    exit(0);
}