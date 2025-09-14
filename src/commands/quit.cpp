/*
 File: src/commands/quit.cpp
 Created: [25.08.2025]
 Command: quit
 Description: Exites the program
 Part-of: built-in
*/

#include <iostream>
#include "../includes/bcolors.h"

void c_quit(const std::vector<std::string>& args = {}) {
    if (!args.empty()) {
        std::string code = args[0];

        for (char c : code) {
            if (!isdigit(c)) {
                setColor(Color::Red);
                std::cout << "[Error] Exit Code accepts only integers\n> Example: quit 5\n";
                setColor(chosenColor);
                return;
            }
        }

        try {
            int codeu = std::stoi(code);
            exit(codeu);
        } catch (...) {
            setColor(Color::Red);
            std::cout << "[Error] Couldn't exit with custom exit code!\n";
            setColor(chosenColor);
            return;
        }
    }

    setColor();
    exit(0);
}
