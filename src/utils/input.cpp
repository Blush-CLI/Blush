#include <utils/input.hpp>
#include <tui/colors.hpp>
#include <core/shell.hpp>
#include <print>

bool isCtrlX(char c, char x) {
    return c == x & 0x1F;
}

std::string Input::input() {
    printPrompt();
    std::string command;
    bool escaped = false;
    bool replace = true;
    int curPos = 0;

    while(true){
        Char c = Input::readkey();
        bool done = false;
        switch(c.escape) {
            case EscapeKey::Escape: {
                escaped = true;
                std::print("\x1b[2K\r[ESCAPE] i: input | r: replace");
            } break;
            case EscapeKey::UpArrow: {
                std::string histCmd = History::getInstance().getPrevious();
                if(!histCmd.empty()) {
                    std::print("\x1b[2K\r");
                    printPrompt();
                    command = histCmd;
                    curPos = command.size();
                    std::print("{}", command);
                }
            } break;
            case EscapeKey::DownArrow: {
                std::string histCmd = History::getInstance().getNext();
                std::print("\x1b[2K\r");
                printPrompt();
                command = histCmd;
                curPos = command.size();
                std::print("{}", command);
            } break;
            case EscapeKey::LeftArrow: {
                if(curPos > 0) {
                    std::print("\b");
                    curPos--;
                }
            } break;
            case EscapeKey::RightArrow: {
                if(curPos < command.size()) {
                    std::print("\x1b[C");
                    curPos++;
                }
            } break;
            case EscapeKey::Backspace: {
                if(curPos > 0 && curPos <= command.size()) {
                    command.erase(curPos - 1, 1);
                    curPos--;
                    std::print("\b \b");
                    if(curPos < command.size()) {
                        std::print("{} \x1b[{}D", command.substr(curPos), command.size() - curPos + 1);
                    }
                }
            } break;

            default: {
                if(escaped) {
                    switch(c.character) {
                        case 'i':
                        case 'I':
                            replace = false;
                            escaped = false;
                            std::print("\x1b[2K\r");
                            printPrompt();
                            std::print("{}", command);
                        break;

                        case 'r':
                        case 'R':
                            replace = true;
                            escaped = false;
                            std::print("\x1b[2K\r");
                            printPrompt();
                            std::print("{}", command);
                        break;
                    }
                    break;
                }

                if(isCtrlX(c.character, 'Q')) {
                    std::println();
                    exit(0);
                }
            
                if(c.character == '\n' || c.character == '\r') {
                    std::println();
                    done = true;
                    break;
                }

                if(std::isprint(static_cast<unsigned char>(c.character))) {
                    if(curPos < command.size()) {
                        if(replace) {
                            command[curPos] = c.character;
                        } else {
                            command.insert(curPos, 1, c.character);
                        }
                        std::print("{}", c.character);
                        if(!replace && curPos < command.size() - 1) {
                            std::print("{}\x1b[{}D", command.substr(curPos + 1), command.size() - curPos - 1);
                        }
                    } else {
                        command += c.character;
                        std::print("{}", c.character);
                    }
                    curPos++;
                }
            } break;
        }
        if(done) break;
    }

    if(!command.empty()) {
        History::getInstance().add(command);
    }

    return command;
}