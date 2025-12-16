#include <commands/base.hpp>
#include <core/shell.hpp>
#include <blush.hpp>
#include <print>

namespace Commands {
    int history(Command command){        
        History* hist = Shell::getHistory();
        if(!hist || hist->isEmpty()) {
            std::println("No command history available.");
            return 0;
        }

        const auto& commands = hist->getCommands();
        for(size_t i = 0; i < commands.size(); i++) {
            std::println("  {}: {}", i + 1, commands[i]);
        }
        
        return 0;
    }
}