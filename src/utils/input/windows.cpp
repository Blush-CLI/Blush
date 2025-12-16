#include <utils/input.hpp>
#include <windows.h>

char Input::readchar() {
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE); // terminal input handle
    INPUT_RECORD rec; // record, holds input data(?)
    DWORD read; // read scancode(?)

    while(true){
        ReadConsoleInput(hIn, &rec, 1, &read);
        if(rec.EventType != KEY_EVENT) continue;
        if(!rec.Event.KeyEvent.bKeyDown) continue;
        if(rec.Event.KeyEvent.uChar.AsciiChar == 0) continue;
        
        return rec.Event.KeyEvent.uChar.AsciiChar;
    }
}

std::string Input::readamount(int n) {
    std::string result;

    for(int i = 0; i < n; i++) {
        result += readchar();
    }
}

Char Input::readkey() {
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE); // terminal input handle
    INPUT_RECORD rec; // record, holds input data(?)
    DWORD read; // read scancode(?)

    while(true){
        ReadConsoleInput(hIn, &rec, 1, &read);
        if(rec.EventType != KEY_EVENT) continue;
        if(!rec.Event.KeyEvent.bKeyDown) continue;

        Char result;

        switch(rec.Event.KeyEvent.wVirtualKeyCode){
            case VK_UP:
                result.escape = EscapeKey::UpArrow;
                return result;
            case VK_DOWN:
                result.escape = EscapeKey::DownArrow;
                return result;
            case VK_LEFT:
                result.escape = EscapeKey::LeftArrow;
                return result;
            case VK_RIGHT:
                result.escape = EscapeKey::RightArrow;
                return result;
            case VK_ESCAPE:
                result.escape = EscapeKey::Escape;
                return result;
            case VK_BACK:
                result.escape = EscapeKey::Backspace;
                return result;
            case VK_DELETE:
                result.escape = EscapeKey::Delete;
                return result;
            case VK_HOME:
                result.escape = EscapeKey::Home;
                return result;
            case VK_END:
                result.escape = EscapeKey::End;
                return result;
            case VK_PRIOR:
                result.escape = EscapeKey::PgUp;
                return result;
            case VK_NEXT:
                result.escape = EscapeKey::PgDn;
                return result;
            default: {
                if(rec.Event.KeyEvent.uChar.AsciiChar != 0){
                    result.character = rec.Event.KeyEvent.uChar.AsciiChar;
                    return result;
                }
            } break;
        }
    }
}