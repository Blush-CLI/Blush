#include <iostream>
#include <vector>
#include "../includes/cmd.h"
#include "../includes/bcolors.h"

void c_help(const std::vector<std::string>& args) {
    setColor(chosenColor);
    std::cout << "\nBlush Built-in Commands:\n";
    setColor();
    for (const auto& [name, func] : commands) {
        setColor(Color::Blue);
        std::cout << "  > ";
        setColor(Color::Magenta);
        std::cout << name << "\n";
        setColor();
    }
}
