#pragma once
#include <iostream>
#include <functional>
#include <map>
#include <string>
#include "./bcolors.h"

void c_cls();
void c_help();
void c_quit();

inline std::map<std::string, std::function<void()>> commands = {
    {"cls", c_cls},
    {"clear", c_cls},
    {"help", c_help},
    {"quit", c_quit},
    {"q", c_quit},
    {"exit", c_quit},
};
