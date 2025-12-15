#include <utils/input.hpp>
#include <windows.h>

char Input::readchar() {
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE); // terminal input handle
    INPUT_RECORD rec; // record, holds input data(?)
    DWORD read; // read scancode(?)

    ReadConsoleInput(hIn, &rec, 1, &read);

    if(rec.EventType == KEY_EVENT){
        return rec.Event.KeyEvent.uChar.AsciiChar; // looks good to me
    }

    return '\0';
}