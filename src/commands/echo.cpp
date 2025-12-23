#include <commands/base.hpp>
#include <utils/file.hpp>
#include <blush.hpp>
#include <string>
#include <sstream>
#include <print>

namespace Commands {
    int echo(Command command){
        D_PRINTLN("Attempting to print...");

        for(int i = 1; i < command.arguments.size(); i++) {
            if(command.arguments[i].starts_with('$')){
                std::string varName = command.arguments[i].substr(1);
                auto varValue = File::getenv(varName.c_str());
                if(varValue.empty() == false){
                    std::print("{} ", varValue);
                } else {
                    std::print(" ");
                }
                continue;
            }
            std::print("{} ", command.arguments[i]);
        }
        std::println();

        return 0;
    }
}