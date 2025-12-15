#include <commands/base.hpp>
#include <blush.hpp>
#include <string>
#include <filesystem>
#include <print>

namespace Commands {
    int cd(Command command){

        try {
            D_PRINTLN("Changing to {}", command.arguments[1]);
            std::filesystem::current_path(command.arguments[1]);
        } catch(std::filesystem::filesystem_error& e){
            std::println("Error: {}", e.what());
            return 1;
        }

        return 0;
    }
}