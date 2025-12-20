#include <utils/input.hpp>
#include <windows.h>
#include <iostream>
#include <functional>

void enableVirtualTerminal() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hOut, &mode);
    SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

Char Input::readkey() {
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD rec;
    DWORD read;

    while (true) {
        ReadConsoleInput(hIn, &rec, 1, &read);
        if (rec.EventType != KEY_EVENT) continue;
        if (!rec.Event.KeyEvent.bKeyDown) continue;

        Char result;

        switch (rec.Event.KeyEvent.wVirtualKeyCode) {
        case VK_UP: result.escape = EscapeKey::UpArrow; return result;
        case VK_DOWN: result.escape = EscapeKey::DownArrow; return result;
        case VK_LEFT: result.escape = EscapeKey::LeftArrow; return result;
        case VK_RIGHT: result.escape = EscapeKey::RightArrow; return result;
        case VK_ESCAPE: result.escape = EscapeKey::Escape; return result;
        case VK_BACK: result.escape = EscapeKey::Backspace; return result;
        case VK_DELETE: result.escape = EscapeKey::Delete; return result;
        case VK_HOME: result.escape = EscapeKey::Home; return result;
        case VK_END: result.escape = EscapeKey::End; return result;
        case VK_PRIOR: result.escape = EscapeKey::PgUp; return result;
        case VK_NEXT: result.escape = EscapeKey::PgDn; return result;
        default:
            if (rec.Event.KeyEvent.uChar.AsciiChar != 0) {
                result.character = rec.Event.KeyEvent.uChar.AsciiChar;
                return result;
            }
        }
    }
}

void clearLine() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hOut, &csbi);
    DWORD written;
    COORD start = {0, csbi.dwCursorPosition.Y};
    FillConsoleOutputCharacter(hOut, ' ', csbi.dwSize.X, start, &written);
    SetConsoleCursorPosition(hOut, start);
}

std::string Input::readLine(History& history, const std::function<void()>& printPrompt) {
    enableVirtualTerminal(); // activate ANSI

    std::string command;
    int curPos = 0;
    bool done = false;

    printPrompt();

    while (!done) {
        Char c = readkey();

        switch (c.escape) {
        case EscapeKey::Escape:
            break;

        case EscapeKey::UpArrow: {
            std::string histCmd = history.getPrevious();
            if (!histCmd.empty()) {
                command = histCmd;
                curPos = command.size();
                std::cout << "\r\x1b[2K";
                printPrompt();
                std::cout << command;
            }
        } break;

        case EscapeKey::DownArrow: {
            std::string histCmd = history.getNext();
            command = histCmd;
            curPos = command.size();
            std::cout << "\r\x1b[2K";
            printPrompt();
            std::cout << command;
        } break;

        case EscapeKey::LeftArrow:
            if (curPos > 0) {
                std::cout << "\x1b[D";
                curPos--;
            }
            break;

        case EscapeKey::RightArrow:
            if (curPos < (int)command.size()) {
                std::cout << "\x1b[C";
                curPos++;
            }
            break;

        case EscapeKey::Backspace:
            if (curPos > 0) {
                command.erase(curPos - 1, 1);
                curPos--;
                std::cout << "\r\x1b[2K";
                printPrompt();
                std::cout << command;
                std::cout << "\r";
                printPrompt();
                std::cout << command.substr(0, curPos);
            }
            break;

        default:
            if (c.character == '\r' || c.character == '\n') {
                std::cout << std::endl;
                done = true;
            } else if (c.character) {
                command.insert(curPos, 1, c.character);
                curPos++;
                std::cout << "\r\x1b[2K";
                printPrompt();
                std::cout << command;
                std::cout << "\r";
                printPrompt();
                std::cout << command.substr(0, curPos);
            }
            break;
        }
    }

    if (!command.empty()) {
        history.add(command);
        history.reset();
    }

    return command;
}

