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

void handleExit(int signal) {
    setColor(Color::Red);
    std::cout << "\nExiting\n";
    setColor();
    exit(0);
}

// ============================================================================
// TERMINAL CONTROL
// ============================================================================

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
    } 
    else if (settingsSaved) {
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

// ============================================================================
// DISPLAY FUNCTIONS
// ============================================================================

void updateCommandLine(const std::string& currentInput, size_t previousLength) {
    std::cout << "\r";
    setColor(Color::Magenta);
    std::cout << "Blush >> ";
    setColor(Color::Blue);
    std::cout << currentInput;
    if (previousLength > currentInput.size()) {
        std::cout << std::string(previousLength - currentInput.size(), ' ');
    }
    std::cout << "\r";
    setColor(Color::Magenta);
    std::cout << "Blush >> ";
    setColor(Color::Blue);
    std::cout << currentInput;
    std::cout.flush();
}

// ============================================================================
// CMD EXEC
// ============================================================================

void runCommand(const std::string& userInput) {
    std::istringstream inputStream(userInput);
    std::string commandName;
    inputStream >> commandName;
    
    std::vector<std::string> arguments;
    std::string argument;
    while (inputStream >> argument) {
        arguments.push_back(argument);
    }
    
    if (!commandName.empty() && commandName[0] == '>') {
        setColor(Color::Yellow, {Style::Bold});
        std::cout << "System: ";
        setColor(chosenColor, {Style::Default});
        
        std::string systemCommand = userInput.substr(1);
        systemCommand = trimWhitespace(systemCommand);
        if (!systemCommand.empty()) {
            system(systemCommand.c_str());
        } else {
            std::cout << "(Blush: no system command provided)\n";
        }
        return;
    }
    
    if (!commandName.empty() && commands.find(commandName) != commands.end()) {
        commands[commandName](arguments);
        addHistory(userInput);
    } else if (!commandName.empty()) {
        setColor(Color::Red);
        std::cerr << "> Unknown command!\n";
        setColor();
    }
}

// ============================================================================
// INPUT HANDLING WITH AUTOCOMPLETE
// ============================================================================

std::string getInputWithTabComplete(std::map<std::string, std::function<void(const std::vector<std::string>&)>>& availableCommands) {
    std::string inputBuffer;
    std::vector<std::string> autocompleteMatches;
    size_t currentMatchIndex = 0;
    size_t previousLineLength = 0;
    
    toggleRawMode(true);
    updateCommandLine(inputBuffer, previousLineLength);
    
    while (true) {
        char keyPressed = readSingleChar();
        
        if (keyPressed == '\n' || keyPressed == '\r') {
            std::cout << "\n";
            break;
        }
        else if (keyPressed == 127 || keyPressed == 8) {
            if (!inputBuffer.empty()) inputBuffer.pop_back();
            autocompleteMatches.clear();
            currentMatchIndex = 0;
        }
        else if (keyPressed == 23) {
            while (!inputBuffer.empty() && inputBuffer.back() == ' ') inputBuffer.pop_back();
            while (!inputBuffer.empty() && inputBuffer.back() != ' ') inputBuffer.pop_back();
            autocompleteMatches.clear();
            currentMatchIndex = 0;
        }
        else if (keyPressed == '\t') {
            if (autocompleteMatches.empty()) {
                for (const auto& commandPair : availableCommands) {
                    if (commandPair.first.find(inputBuffer) == 0) {
                        autocompleteMatches.push_back(commandPair.first);
                    }
                }
                currentMatchIndex = 0;
            } 
            else {
                currentMatchIndex = (currentMatchIndex + 1) % autocompleteMatches.size();
            }
            
            if (!autocompleteMatches.empty()) {
                inputBuffer = autocompleteMatches[currentMatchIndex];
            }
        }
        else if (isprint(keyPressed)) {
            inputBuffer += keyPressed;
            autocompleteMatches.clear();
            currentMatchIndex = 0;
        }
        
        updateCommandLine(inputBuffer, previousLineLength);
        previousLineLength = inputBuffer.size();
    }
    
    toggleRawMode(false);
    return inputBuffer;
}

int main() {
    signal(SIGINT, handleExit);

    std::string bsucolor = getConfigString("startUp{}", "bsucolor");
    chosenColor = stringToColor(bsucolor);
    
    while (true) {
        setColor(Color::Magenta);
        std::cout << "Blush >> ";
        
        std::string userInput = getInputWithTabComplete(commands);
        userInput = trimWhitespace(userInput);
        
        if (!userInput.empty()) {
            runCommand(userInput);
        }
    }
    
    return 0;
}
