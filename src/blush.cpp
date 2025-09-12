/*
 * ============================================================
 *  >> Main File
 *  File:        src/blush.cpp
 *  Created:     [25.08.2025]
 *  Description: Main logic CLI
 * ============================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <csignal>
#include <sstream>
#include <functional>

#ifdef _WIN32
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif

#include "./includes/cmd.h"
#include "./includes/bcolors.h"
#include "./includes/iojson.h"
#include "./includes/trim.h"
#include "./includes/historyhandler.h"
#include "./includes/s_upu.h"

void handleExit(int signal) {
    setColor(Color::Red);
    std::cout << "\nExiting\n";
    setColor();
    exit(0);
}

void toggleRawMode(bool enable) {
#ifndef _WIN32
    static struct termios originalSettings;
    static bool settingsSaved = false;

    if (enable) {
        if (!settingsSaved) {
            tcgetattr(STDIN_FILENO, &originalSettings);
            settingsSaved = true;
        }

        struct termios rawSettings = originalSettings;
        rawSettings.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &rawSettings);
    } else if (settingsSaved) {
        tcsetattr(STDIN_FILENO, TCSANOW, &originalSettings);
    }
#endif
}

char readSingleChar() {
#ifdef _WIN32
    return static_cast<char>(_getch());
#else
    char character;
    read(STDIN_FILENO, &character, 1);
    return character;
#endif
}

void updateCommandLine(const std::string& currentInput, size_t previousLength, int historyIndex, int processSize, int fileSize) {
    std::cout << "\r";
    std::cout << std::string(previousLength + 40, ' ');
    std::cout << "\r";

    setColor(Color::Magenta);
    std::cout << "Blush >> ";
    setColor(Color::Blue);
    std::cout << currentInput;

    if (historyIndex > 0) {
        if (historyIndex <= processSize) {
            setColor(Color::Blue, {Style::Bold});
            std::cout << "  [History: Process]";
        } else if (historyIndex <= processSize + fileSize) {
            setColor(Color::Green, {Style::Bold});
            std::cout << "  [History: config.json]";
        }
    }

    setColor(Color::Blue, {Style::Default});
    std::cout.flush();
}

void runCommand(const std::string& userInput, std::vector<std::string>& processHistory) {
    swn("Blush - Executing");

    std::istringstream inputStream(userInput);
    std::string commandName;
    inputStream >> commandName;

    std::vector<std::string> arguments;
    std::string argument;
    while (inputStream >> argument) {
        arguments.push_back(argument);
    }

    if (!commandName.empty() && commandName[0] == '>') {
        if (commandName == ">blush") {
            char choice = '\0';
            while (choice != 'y' && choice != 'Y' && choice != 'n' && choice != 'N') {
                setColor(Color::Red);
                c_cls();
                std::cout << "Executing Blush within Blush may cause unknown bugs!\n"
                          << "Do you want to continue? (y/n): ";
                std::cin >> choice;
                if (choice == '\n' || choice == '\r') choice = '\0';
            }

            if (choice != 'y' && choice != 'Y') {
                setColor(Color::Green);
                std::cout << "\nAborted.\n";
                setColor();
                return;
            }
        }

        setColor(Color::Yellow, {Style::Bold});
        std::cout << "System: ";
        setColor(chosenColor, {Style::Default});

        std::string systemCommand = trimWhitespace(userInput.substr(1));
        if (!systemCommand.empty()) {
            std::cout << "\n";
            system(systemCommand.c_str());
        } else {
            std::cout << "(Blush: no system command provided)\n";
        }

        std::cout << "\n";
        return;
    }

    if (!commandName.empty() && commands.find(commandName) != commands.end()) {
        commands[commandName].func(arguments);
        addHistory(userInput);
        processHistory.push_back(userInput);
    } else if (!commandName.empty()) {
        setColor(Color::Red);
        std::cerr << "> Unknown command!\n";
        setColor();
    }
}

std::string getInput(std::map<std::string, Command>& availableCommands, std::vector<std::string>& processHistory) {
    std::string inputBuffer;
    std::vector<std::string> autocompleteMatches;
    size_t currentMatchIndex = 0;
    size_t previousLineLength = 0;

    std::vector<std::string> fileHistory = getHistory();
    int processSize = static_cast<int>(processHistory.size());
    int fileSize = static_cast<int>(fileHistory.size());
    int historyIndex = 0;

    toggleRawMode(true);
    updateCommandLine(inputBuffer, previousLineLength, historyIndex, processSize, fileSize);

    while (true) {
        char keyPressed = readSingleChar();

        if (keyPressed == '\n' || keyPressed == '\r') {
            std::cout << "\n";
            break;
        } else if (keyPressed == 127 || keyPressed == 8) {
            if (!inputBuffer.empty()) inputBuffer.pop_back();
            autocompleteMatches.clear();
            currentMatchIndex = 0;
            historyIndex = 0;
        } else if (keyPressed == 23) {
            while (!inputBuffer.empty() && inputBuffer.back() == ' ') inputBuffer.pop_back();
            while (!inputBuffer.empty() && inputBuffer.back() != ' ') inputBuffer.pop_back();
            autocompleteMatches.clear();
            currentMatchIndex = 0;
            historyIndex = 0;
        } else if (keyPressed == '\t') {
            if (autocompleteMatches.empty()) {
                for (const auto& commandPair : availableCommands) {
                    if (commandPair.first.find(inputBuffer) == 0) {
                        autocompleteMatches.push_back(commandPair.first);
                    }
                }
                currentMatchIndex = 0;
            } else {
                currentMatchIndex = (currentMatchIndex + 1) % autocompleteMatches.size();
            }

            if (!autocompleteMatches.empty()) {
                inputBuffer = autocompleteMatches[currentMatchIndex];
                historyIndex = 0;
            }
        } else if (keyPressed == '\033') {
            char seq[2];
            if (read(STDIN_FILENO, &seq[0], 1) == 0) continue;
            if (read(STDIN_FILENO, &seq[1], 1) == 0) continue;

            if (seq[0] == '[') {
                if (seq[1] == 'A') {
                    if (historyIndex < processSize + fileSize) historyIndex++;
                    if (historyIndex <= processSize) {
                        inputBuffer = processHistory[processSize - historyIndex];
                    } else if (historyIndex <= processSize + fileSize) {
                        inputBuffer = fileHistory[fileSize - (historyIndex - processSize)];
                    }
                } else if (seq[1] == 'B') {
                    if (historyIndex > 0) historyIndex--;
                    if (historyIndex == 0) {
                        inputBuffer.clear();
                    } else if (historyIndex <= processSize) {
                        inputBuffer = processHistory[processSize - historyIndex];
                    } else {
                        inputBuffer = fileHistory[fileSize - (historyIndex - processSize)];
                    }
                }
            }
        } else if (isprint(keyPressed)) {
            inputBuffer += keyPressed;
            autocompleteMatches.clear();
            currentMatchIndex = 0;
            historyIndex = 0;
        }

        updateCommandLine(inputBuffer, previousLineLength, historyIndex, processSize, fileSize);
        previousLineLength = inputBuffer.size();
    }

    toggleRawMode(false);
    return inputBuffer;
}

int main() {
    signal(SIGINT, handleExit);
    startm();

    std::string bsucolor = getConfigString("startUp{}", "bsucolor");
    chosenColor = stringToColor(bsucolor);
    std::vector<std::string> processHistory;

    while (true) {
        setColor(Color::Magenta);
        std::cout << "Blush >> ";

        std::string userInput = getInput(commands, processHistory);
        userInput = trimWhitespace(userInput);

        if (!userInput.empty()) {
            runCommand(userInput, processHistory);
            sdwt();
        }
    }

    return 0;
}
