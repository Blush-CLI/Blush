#include <commands/base.hpp>
#include <blush.hpp>
#include <string>
#include <sstream>
#include <print>

namespace Commands {
    int echo(Command command){
        D_PRINTLN("yes bro");
        bool a = false;
        for(auto& arg : command.arguments) {
            if(!a) {
                a = true;
                continue;
            }
            std::print("{} ", arg);
        }
        std::println();

        return 0;
    }
}