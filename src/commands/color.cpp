/*
 File: src/commands/color.cpp
 Created: [03.09.2025]
 Command: color
 Description: Let's you select a color, save it to config & display a color list
 Part-of: built-in
*/

#include <iostream>
#include <vector>
#include <string>
#include "../includes/bcolors.h"
#include "../includes/iojson.h"

Color stringToColor(const std::string& str) {
    std::string lower;
    lower.reserve(str.size());
    for (char c : str) lower += std::tolower(c);

    if (lower == "default") return Color::Default;
    if (lower == "black")   return Color::Black;
    if (lower == "red")     return Color::Red;
    if (lower == "green")   return Color::Green;
    if (lower == "yellow")  return Color::Yellow;
    if (lower == "blue")    return Color::Blue;
    if (lower == "magenta") return Color::Magenta;
    if (lower == "cyan")    return Color::Cyan;
    if (lower == "white")   return Color::White;

    return Color::Default; // fallback
}

std::string colorToString(Color color) {
    switch (color) {
        case Color::Default: return "Default";
        case Color::Black:   return "Black";
        case Color::Red:     return "Red";
        case Color::Green:   return "Green";
        case Color::Yellow:  return "Yellow";
        case Color::Blue:    return "Blue";
        case Color::Magenta: return "Magenta";
        case Color::Cyan:    return "Cyan";
        case Color::White:   return "White";
        default:             return "Unknown";
    }
}

void c_color(const std::vector<std::string>& args) {
    setColor();
    if (args.empty()) {
        setColor(Color::Red);
        std::cout << "Please provide a color! <e.g: Red, Green, Blue>\n";
        setColor(chosenColor);
        return;
    }

    if (args[0] == "which") {
        setColor(chosenColor);
        std::cout << "Selected Color: " << colorToString(chosenColor) << "\n";
        return;
    } else if (args[0] == "list") {
        for (Color color : allColors) {
            std::cout << "  > ";
            setColor(color);
            std::cout << colorToString(color) << "\n";
            setColor(Color::Default);
        }
        return;
    }

    std::string arg = args[0];
    if (arg == "Red" || arg == "red") chosenColor = Color::Red;
    else if (arg == "Green" || arg == "green") chosenColor = Color::Green;
    else if (arg == "Blue" || arg == "blue") chosenColor = Color::Blue;
    else if (arg == "Yellow" || arg == "yellow") chosenColor = Color::Yellow;
    else if (arg == "Magenta" || arg == "magenta") chosenColor = Color::Magenta;
    else if (arg == "Cyan" || arg == "cyan") chosenColor = Color::Cyan;
    else if (arg == "White" || arg == "white") chosenColor = Color::White;
    else if (arg == "Black" || arg == "black") chosenColor = Color::Black;
    else {
        Color tempColor = chosenColor;
        setColor(Color::Red);
        std::cout << "> Unknown color: " << arg << "\n";
        setColor(tempColor);
        return;
    }

    // Save to config, load on startup
    if (args.size() > 1 && args[1] == "save") {
        saveConfig("startUp{}", "bsucolor", args[0]);
        setColor(chosenColor);
        std::cout << "Color set to " << arg << " and will auto load on start\n";
        setColor();
        return;
    }

    // Set the color (for this time only)
    setColor(chosenColor);
    std::cout << "Color set to " << arg << "\n";
}
