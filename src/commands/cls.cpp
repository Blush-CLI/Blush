#include <iostream>
#include "../includes/cmd.h"

void c_cls() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear")
    #endif
}