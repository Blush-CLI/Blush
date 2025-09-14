/*
 File: src/commands/alias.cpp
 Created: [14.09.2025]
 Command: alias
 Description: Manages command aliases
 Part-of: built-in
*/

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "../includes/cmd.h"
#include "../includes/bcolors.h"

static std::map<std::string, std::pair<std::string, std::string>> aliases;

void c_alias(const std::vector<std::string>& args) {
    if (args.empty()) {
        setColor(Color::Red);
        std::cout << "[U] Usage: alias set/unset <command> <alias> <executor>\n[I] See ";
        setColor(Color::Red, {Style::Underline, Style::Italic});
        std::cout << "https://docs.getblush.xyz/commands/alias";
        setColor(Color::Red, {Style::Default});
        std::cout << " for help.\n" << std::endl;
        return;
    }

    if (args[0] == "set" && args.size() == 4) {
        std::string command = args[1];
        std::string alias_name = args[2];
        std::string executor = args[3];
        
        if (executor != "b" && executor != "sys") {
            setColor(Color::Red);
            std::cout << "[E] Executor must be 'b' (blush) or 'sys' (system)" << std::endl;
            return;
        }
        
        aliases[alias_name] = std::make_pair(command, executor);
        setColor(Color::Green);
        std::cout << "[S] Alias '" << alias_name << "' set for command '" << command << "' with executor '" << executor << "'" << std::endl;
    }
    else if (args[0] == "unset" && args.size() == 2) {
        std::string alias_name = args[1];
        
        auto it = aliases.find(alias_name);
        if (it != aliases.end()) {
            aliases.erase(it);
            setColor(Color::Green);
            std::cout << "[S] Alias '" << alias_name << "' removed" << std::endl;
        } else {
            setColor(Color::Red);
            std::cout << "[E] Alias '" << alias_name << "' not found" << std::endl;
        }
    }
    else if (args[0] == "list") {
        if (aliases.empty()) {
            setColor(Color::Yellow);
            std::cout << "[I] No aliases set" << std::endl;
        } else {
            setColor(Color::Blue);
            std::cout << "[I] Current aliases:" << std::endl;
            for (const auto& alias : aliases) {
                std::cout << "  " << alias.first << " -> " << alias.second.first << " (" << alias.second.second << ")" << std::endl;
            }
        }
    }
    else {
        setColor(Color::Red);
        std::cout << "[U] Usage: alias set/unset <command> <alias> <executor>\n[I] See ";
        setColor(Color::Red, {Style::Underline, Style::Italic});
        std::cout << "https://docs.getblush.xyz/commands/alias";
        setColor(Color::Red, {Style::Default});
        std::cout << " for help.\n" << std::endl;
    }
}