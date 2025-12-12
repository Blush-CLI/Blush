/*
 File: src/commands/cls.cpp
 Created: [28.08.2025]
 Command: cls
 Description: Basic lol clears the screen
 Part-of: built-in
*/

#include <iostream>
#include "../includes/cmd.h"

void c_cls(const std::vector<std::string>& args) {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}
