#include <commands/base.hpp>
#include <blush.hpp>
#include <string>
#include <sstream>
#include <print>

namespace Commands {
    int clear(Command command){
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        return 0;
    }
}