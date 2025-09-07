/*
 File: src/commands/history.cpp
 Created: [28.08.2025]
 Command: history
 Description: Retrieves history from json file in config dir and displays it
 Part-of: built-in
*/

#include <iostream>
#include <vector>
#include <string>
#include "../includes/historyhandler.h"
#include "../includes/bcolors.h"

std::vector<std::string> getHistory();

void c_history(const std::vector<std::string>& args) {
    setColor(chosenColor);
    std::cout << "Showing last 100 commands:\n";
    printHistory();
}