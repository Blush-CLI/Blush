/*
 File: src/utils/startup.cpp
 Created: [25.08.2025]
 Description: Does some things on startup
*/

#include <iostream>
#include "../includes/s_upu.h"
#include "../includes/loanim.h"
#include "../includes/cmd.h"

#ifdef _WIN32
    #include <windows.h>
#elif __linux__
    #include <sys/prctl.h>
    #include <unistd.h>
#endif

std::string wd_title   = "Blush - Ready!";   // Default Startup Window name
std::string t_process = "blush";             // Process name

void loadAliasesFromConfig();

void sdwnofsu() {
#ifdef _WIN32
    SetConsoleTitleA(wd_title.c_str());
#elif __linux__
    prctl(PR_SET_NAME, t_process.c_str(), 0, 0, 0);
    std::cout << "\033]0;" << wd_title << "\007";
    std::cout.flush();
#endif
    _la_animstop();
}

void sdwt() {
#ifdef _WIN32
    SetConsoleTitleA(wd_title.c_str());
#elif __linux__
    prctl(PR_SET_NAME, t_process.c_str(), 0, 0, 0);
    std::cout << "\033]0;" << wd_title << "\007";
    std::cout.flush();
#endif
}

void swn(std::string title) {
#ifdef _WIN32
    SetConsoleTitleA(title.c_str());
#elif __linux__
    std::cout << "\033]0;" << title << "\007";
    std::cout.flush();
#endif
}

void startm() {
    loadAliasesFromConfig();
    sdwnofsu();
}