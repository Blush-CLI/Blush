#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "../includes/cmd.h"
#include "../includes/bcolors.h"
#include "../utils/json.hpp"
#include "../includes/iojson.h"

static std::map<std::string, std::pair<std::string, std::string>> aliases;

void saveAliasesToConfig() {
    nlohmann::json alias_json;
    for (const auto& [alias_name, data] : aliases) {
        alias_json[alias_name] = {{"command", data.first}, {"executor", data.second}};
    }
    nlohmann::json j = loadConfig();
    j["aliases"] = alias_json;
    saveConfigToFile(j);
}

void loadAliasesFromConfig() {
    nlohmann::json j = loadConfig();
    if (j.contains("aliases") && j["aliases"].is_object()) {
        for (auto& [alias_name, data] : j["aliases"].items()) {
            if (data.contains("command") && data.contains("executor")) {
                aliases[alias_name] = std::make_pair(data["command"].get<std::string>(), data["executor"].get<std::string>());
            }
        }
    }
}

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
        saveAliasesToConfig();
        setColor(Color::Green);
        std::cout << "[S] Alias '" << alias_name << "' set for command '" << command << "' with executor '" << executor << "'" << std::endl;
    }
    else if (args[0] == "unset" && args.size() == 2) {
        std::string alias_name = args[1];

        auto it = aliases.find(alias_name);
        if (it != aliases.end()) {
            aliases.erase(it);
            saveAliasesToConfig();
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

std::map<std::string, std::pair<std::string, std::string>>& getAliases() {
    return aliases;
}
