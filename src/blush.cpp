#include <iostream>
#include <string>
#include <map>
#include <functional>
#include ".\includes\bbcmds.h"
#include ".\includes\bcolors.h"

void helpCommand() {
    setColor(Color::Green);
    std::cout << "\nBlush Built-in Commands:\n";
    setColor();
    for (const auto& [name, func] : bbcommands) {
        setColor(Color::Blue);
        std::cout << "  > ";
        setColor(Color::Magenta);
        std::cout << name << "\n";
        setColor();
    }
    setColor();
}

void aboutCommand() {
    std::cout << "About command in the future implementation\n";
}

void versionCommand() {
    std::cout << "Version command in the future implementation\n";
}

void authorCommand() {
    std::cout << "Author command in the future implementation\n";
}

bool ifInArray(const std::string &check) {
    return bbcommands.find(check) != bbcommands.end();
}

void executeByCommand(const std::string &command) {
    if (ifInArray(command)) {
        bbcommands[command]();
    } else {
        setColor(Color::Red);
        std::cerr << "Unknown command!\n";
        setColor();
    }
}

int main() {
    std::string input;

    while (true) {
        setColor(Color::Magenta);
        std::cout << "Blush >> ";
        setColor(Color::Blue);
        std::getline(std::cin, input);

        if (input.empty()) continue;
        if (input == "exit" || input == "quit" || input == "q") {
            setColor();
            break;
        };

        setColor();
        executeByCommand(input);
        std::cout << "\n";
    }
}
