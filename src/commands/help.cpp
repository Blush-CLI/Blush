#include <commands/base.hpp>
#include <tui/colors.hpp>
#include <print>
#include <map>

std::map<std::string, std::string_view> Texts = {
    { "> Hi, Thanks for using the Help command!", Color::Cyan },
    { "  All Built-in Commands available are listed below:", Color::Cyan }
};

namespace Commands {
    int help(Command command){
        for (auto Text = Texts.rbegin(); Text != Texts.rend(); ++Text) {
            std::print("{}{}\n", Text->second, Text->first);
        } // Print Strings from Texts Map with thier Color

        for (auto& [name, exp] : commands_map) {
            std::print("{}   - {} / {}\n", Color::Bold, name, exp.description);
        } // Print Commands with thier Description
        
        return 0;
    }
}