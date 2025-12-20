#include <commands/base.hpp>
#include <tui/colors.hpp>
#include <utils/file.hpp>
#include <blush.hpp>
#include <string>
#include <filesystem>
#include <print>

namespace Commands {
    int cd(Command command){
        D_PRINTLN("Changing to {}", command.arguments[1]);
        try {
            if (command.arguments.size() < 2) {
                std::filesystem::current_path(File::getHome());
                return 0;
            }
            std::filesystem::current_path(command.arguments[1]);
        } catch(std::filesystem::filesystem_error& e){
            std::println("Error: {}", e.what());
            return 1;
        }

        return 0;
    }
}