#include <iostream>
#include <string>
#include "./includes/cmd.h"
#include "./includes/bcolors.h"
#include <signal.h>
#include <csignal>
#include <sstream>

void beforeExit(int sigint) {
    setColor(Color::Red);
    std::cout << "\n\nExiting\n";
    std::cout.flush();
    setColor();
    exit(0);
}

std::string trim(const std::string &s) {
    size_t start = s.find_first_not_of(" \r\n\t");
    size_t end = s.find_last_not_of(" \r\n\t");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

bool ifInArray(const std::string &check) {
    return commands.find(check) != commands.end();
}

void executeByCommand(const std::string &input) {
    std::istringstream iss(input);
    std::string cmd;
    iss >> cmd;

    std::vector<std::string> args;
    std::string arg;
    while (iss >> arg) args.push_back(arg);

    if (commands.find(cmd) != commands.end()) {
        commands[cmd](args);
    } else {
        setColor(Color::Red);
        std::cerr << "Unknown command!\n";
        setColor();
    }
}

int main() {
    signal(SIGINT, beforeExit);
    std::string input;
    
    while (true) {
        setColor(Color::Magenta);
        std::cout << "Blush >> ";
        setColor(Color::Blue);
        
        if (!std::getline(std::cin, input)) {
            if (std::cin.eof()) {
                c_quit();
            }
            std::cin.clear();
            continue;
        }

        input = trim(input);
        if (input.empty()) continue;

        executeByCommand(input);
        std::cout << "\n";
    }
    
    return 0;
}