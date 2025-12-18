#include <commands/base.hpp>
#include <tui/colors.hpp>
#include <blush.hpp>
#include <string>
#include <filesystem>
#include <print>

namespace Commands {
    int cd(Command command){
        try {
            if (command.arguments.size() < 2) {
                std::println("{}Error: Missing Arguments [Required: 1, Got: {}]", Color::Red, command.arguments.size() - 1);
                return 1;
            };
            D_PRINTLN("Changing to {}", command.arguments[1]);
            std::filesystem::current_path(command.arguments[1]);
        } catch(std::filesystem::filesystem_error& e){
            std::println("Error: {}", e.what());
            return 1;
        }

        return 0;
    }
}