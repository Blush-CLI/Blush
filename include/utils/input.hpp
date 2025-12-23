#pragma once
#include <string>

enum class EscapeKey {
    None,
    UpArrow,
    DownArrow,
    LeftArrow,
    RightArrow,
    Escape,
    Backspace,
    Delete,
    Home,
    End,
    PgUp,
    PgDn
};

struct Char {
    char character;
    EscapeKey escape;

    Char(char c = '\0', EscapeKey esc = EscapeKey::None)
        : character(c), escape(esc) {}
};

class Input {
public:
    static char readchar();
    static std::string readamount(int n);
    static Char readkey();
    static std::string input();
}; // code