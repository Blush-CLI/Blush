#include <iostream>
#include <string>
#include <map>
#include <functional>

void helpCommand();
void aboutCommand();
void versionCommand();
void authorCommand();

std::map<std::string, std::function<void()>> bbcommands = {
    {"help", helpCommand},
    {"cmds", helpCommand},
    {"about", aboutCommand},
    {"version", versionCommand},
    {"author", authorCommand}
};

void helpCommand() {
    std::cout << "Blush Built-in Commands:\n";
    for (const auto& [name, func] : bbcommands) {
        std::cout << "  > " << name << "\n";
    }
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
    }
}

int main(int argc, char* argv[]) {
    if (argc > 1 && !ifInArray(argv[1])) {
        std::cout << "Unknown command!\n";
    } else if (argc > 1) {
        executeByCommand(argv[1]);
    } else {
        std::cout << "No command provided\n";
    }
}
