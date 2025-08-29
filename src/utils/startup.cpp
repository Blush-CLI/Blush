#include <iostream>
#include "../includes/s_upu.h"

#ifdef _WIN32
    #include <windows.h>
#elif __linux__
    #include <sys/prctl.h>
    #include <unistd.h>
#endif

std::string wd_title   = "Blush - Ready!";   // Default Startup Window name
std::string t_process = "blush";             // Process name

void sdwt() {
#ifdef _WIN32
    SetConsoleTitleA(wd_title.c_str());
#elif __linux__
    // change process name
    prctl(PR_SET_NAME, t_process.c_str(), 0, 0, 0);
    // change window name
    std::cout << "\033]0;" << wd_title << "\007";
    std::cout.flush();
#endif
} // sdwt = Set default window name

void swn(std::string title) {
#ifdef _WIN32
    SetConsoleTitleA(title.c_str());
#elif __linux__
    std::cout << "\033]0;" << title << "\007";
    std::cout.flush();
#endif
} // swn = Set window name

void startm() {
    sdwt();
} // Execute on startup