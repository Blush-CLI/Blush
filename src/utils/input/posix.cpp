#include <utils/input.hpp>
#include <termios.h>
#include <unistd.h>
#include <cstdio>

char Input::readchar() {
    struct termios oldTerm, newTerm;
    
    tcgetattr(STDIN_FILENO, &oldTerm);
    newTerm = oldTerm;
    
    newTerm.c_lflag &= ~(ICANON | ECHO | ISIG);
    newTerm.c_iflag &= ~(IXON | IXOFF | IXANY);

    newTerm.c_cc[VMIN] = 1;
    newTerm.c_cc[VTIME] = 0;    

    tcsetattr(STDIN_FILENO, TCSANOW, &newTerm);
    
    char ch = getchar();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldTerm);
    
    return ch;
}

std::string Input::readamount(int n) {
    struct termios oldTerm, newTerm;
    
    tcgetattr(STDIN_FILENO, &oldTerm);
    newTerm = oldTerm;
    
    newTerm.c_lflag &= ~(ICANON | ECHO | ISIG);
    newTerm.c_iflag &= ~(IXON | IXOFF | IXANY);

    newTerm.c_cc[VMIN] = 1;
    newTerm.c_cc[VTIME] = 0;
    
    tcsetattr(STDIN_FILENO, TCSANOW, &newTerm);
    
    std::string result;
    for (int i = 0; i < n; i++) {
        char ch = getchar();
        if (ch == EOF) break;
        result += ch;
    }
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldTerm);
    
    return result;
}

Char Input::readkey() {
    struct termios oldTerm, newTerm;
    
    tcgetattr(STDIN_FILENO, &oldTerm);
    newTerm = oldTerm;
    
    newTerm.c_lflag &= ~(ICANON | ECHO | ISIG);
    newTerm.c_iflag &= ~(IXON | IXOFF | IXANY);
    newTerm.c_cc[VMIN] = 1;
    newTerm.c_cc[VTIME] = 0;
    
    tcsetattr(STDIN_FILENO, TCSANOW, &newTerm);
    
    char ch = getchar();
    Char result;
    
    if (ch == 27) {
        newTerm.c_cc[VMIN] = 0;
        newTerm.c_cc[VTIME] = 1;
        tcsetattr(STDIN_FILENO, TCSANOW, &newTerm);
        
        char next = getchar();
        if (next == '[') {
            char arrow = getchar();
            switch (arrow) {
                case 'A': result.escape = EscapeKey::UpArrow; break;
                case 'B': result.escape = EscapeKey::DownArrow; break;
                case 'C': result.escape = EscapeKey::RightArrow; break;
                case 'D': result.escape = EscapeKey::LeftArrow; break;
                case '1':
                    if (getchar() == '~') result.escape = EscapeKey::Home;
                    break;
                case '3':
                    if (getchar() == '~') result.escape = EscapeKey::Delete;
                    break;
                case '4':
                    if (getchar() == '~') result.escape = EscapeKey::End;
                    break;
                case '5':
                    if (getchar() == '~') result.escape = EscapeKey::PgUp;
                    break;
                case '6':
                    if (getchar() == '~') result.escape = EscapeKey::PgDn;
                    break;
            }
        } else {
            result.escape = EscapeKey::Escape;
        }
    } else {
        result.character = ch;
        
        if (ch == 127 || ch == 8) {
            result.escape = EscapeKey::Backspace;
        }
    }
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldTerm);
    return result;
}