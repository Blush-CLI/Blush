#include <iostream>
#include "../includes/cmd.h"
#include "../includes/bcolors.h"

void c_help() {
    setColor(Color::Green);
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
