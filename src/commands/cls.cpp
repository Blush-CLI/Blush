#include <iostream>
#include "../includes/cmd.h"

void c_cls(const std::vector<std::string>& args) {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear")
    #endif
}