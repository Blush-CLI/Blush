#include "includes/bcolors.h"
#include <iostream>

std::string colorCode(Color color) {
    switch(color) {
        case Color::Black:   return "30";
        case Color::Red:     return "31";
        case Color::Green:   return "32";
        case Color::Yellow:  return "33";
        case Color::Blue:    return "34";
        case Color::Magenta: return "35";
        case Color::Cyan:    return "36";
        case Color::White:   return "37";
        default:             return "39";
    }
}

std::string styleCode(Style style) {
    switch(style) {
        case Style::Bold:      return "1";
        case Style::Italic:    return "3";
        case Style::Underline: return "4";
        case Style::Reversed:  return "7";
        default:               return "0";
    }
}

void setColor(Color color, const std::vector<Style>& styles, const std::string& font) {
    std::string code = "\033[";
    for (size_t i = 0; i < styles.size(); ++i) {
        code += styleCode(styles[i]);
        if (i + 1 < styles.size()) code += ";";
    }
    if (!styles.empty()) code += ";";
    code += colorCode(color) + "m";
    std::cout << code;
}
