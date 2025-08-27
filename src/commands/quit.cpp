#include <iostream>
#include "../includes/bcolors.h"

void c_quit(const std::vector<std::string>& args = {}) {
    setColor();
    exit(0);
}