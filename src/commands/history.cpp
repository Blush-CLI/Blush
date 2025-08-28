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