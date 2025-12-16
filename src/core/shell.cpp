#include <commands/base.hpp>
#include <core/shell.hpp>
#include <core/job.hpp>
#include <core/history.hpp>
#include <utils/file.hpp>
#include <utils/input.hpp>
#include <tui/colors.hpp>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <print>
#include <blush.hpp>

History* Shell::currentHistory = nullptr;

void Shell::Init() {
    currentHistory = &history;
    std::println("{}[  OK  ]{} Shell initialized!{}",
        Color::Green, 
        Color::Purple,
        Color::Reset);
    int rcode = run();
    D_PRINTLN("Shell process exited with code {}", rcode);
}

History* Shell::getHistory() {
    return currentHistory;
}

void printPrompt() {
    std::cout
            << Color::Purple
            << "["
            << File::lcwd()
            << "]"
            << " "
            << Color::Blue
            << "> "
            << Color::Reset
            << std::flush;
}

int Shell::run() {
    audio.play("music/maybe_blush.ogg");
    while(true){
        printPrompt();
        std::string command; // it sounds like a kick
        bool escaped = false;
        bool replace = true;
        int curPos = 0;

        while(true){
            Char c = Input::readkey();
            bool done = false;
            switch(c.escape) {
                case EscapeKey::Escape: {
                    escaped = true;
                    std::cout << "\x1b[2K\r[ESCAPE] i: input | r: replace" << std::flush;
                } break; // can u try running this on real console and test escape
                case EscapeKey::UpArrow: {
                    std::string histCmd = history.getPrevious();
                    if(!histCmd.empty()) {
                        std::cout << "\x1b[2K\r" << std::flush;
                        printPrompt();
                        command = histCmd;
                        curPos = command.size();
                        std::cout << command << std::flush;
                    }
                } break;
                case EscapeKey::DownArrow: {
                    std::string histCmd = history.getNext();
                    std::cout << "\x1b[2K\r" << std::flush;
                    printPrompt();
                    command = histCmd;
                    curPos = command.size();
                    std::cout << command << std::flush;
                } break;
                case EscapeKey::LeftArrow: {
                    if(curPos > 0) {
                        std::cout << "\b";
                        curPos--;
                    }
                } break;
                case EscapeKey::RightArrow: {
                    if(curPos < command.size()) {
                        std::cout << "\x1b[C";
                        curPos++;
                    }
                } break;
                case EscapeKey::Backspace: {
                    if(curPos > 0) {
                        command.pop_back();
                        curPos--;
                        std::print("\b \b");
                    }
                } break;

                default: {

                    if(escaped) {
                        switch(c.character) {
                            case 'i':
                            case 'I':
                                replace = false;
                                escaped = false;
                                std::cout << "\x1b[2K\r" << std::flush;
                                printPrompt();
                                std::cout << command;
                            break;
                                
                            case 'r':
                            case 'R':
                                replace = true;
                                escaped = false;
                                std::cout << "\x1b[2K\r" << std::flush;
                                printPrompt();
                                std::cout << command;
                            break;
                        }
                        break;
                    }

                    switch(c.character + 'A' - 1){
                        case 'Q': // Ctrl + Q  
                            std::println();
                            exit(0);
                    }
        
                    if(c.character == '\n' || (c.character == '\r')) {
                        std::println();
                        done = true;
                        break;
                    }
        
                    command += c.character;
                    std::cout << c.character << std::flush; // yes backspace is still something
                }
            }
            if(done) break;
        }

        try {
            if(command.empty()) continue;
            
            history.add(command);
            history.reset();
            
            Command cmd(command);

            if(cmd.mainCommand == "exit") exit(0);
            
            if(commands_map.find(cmd.mainCommand) != commands_map.end()) {
                commands_map[cmd.mainCommand].func(cmd);
            } else {
                Job job(command);
                bool result = job.run();
                if(!result) {
                    audio.play("music/error.ogg");
                    std::println("{}Error: '{}{}{}{}' | Command not found. \
                        \n{}> Exit code: {}",
                        Color::Red, Color::Bold, cmd.mainCommand, Color::Reset, Color::Red, Color::Yellow, job.exitCode);
                } // alt up arrow
            }
        } catch (std::exception& e) {
            std::println("An error occured: {}", e.what());
        }
    }
    return 0;
}

Shell::~Shell() {
    std::println("Goodbye!");
}