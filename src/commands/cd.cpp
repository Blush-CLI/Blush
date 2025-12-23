#include <commands/base.hpp>
#include <tui/colors.hpp>
#include <utils/file.hpp>
#include <blush.hpp>
#include <string>
#include <filesystem>
#include <print>

namespace Commands {
    int cd(Command command){
        std::string gotoPath;
        if (command.arguments.size() < 2) gotoPath = File::getHome();
        else gotoPath = command.arguments[1];
        
        D_PRINTLN("Changing to {}", gotoPath);
        try {
            std::filesystem::current_path(gotoPath);
        } catch(std::filesystem::filesystem_error& e){
            std::println("Error: {}", e.what());
            return 1;
        }

        return 0;
    }
}