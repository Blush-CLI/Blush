/*
 File: src/includes/bcolors.h
 Created: [25.08.2025]
 Description: Color functions and etc here
*/

#pragma once
#include <string>
#include <vector>

enum class Color { Default, Black, Red, Green, Yellow, Blue, Magenta, Cyan, White };
enum class Style { Default, Bold, Italic, Underline, Reversed };

const std::vector<Color> allColors = {
    Color::Default, Color::Black, Color::Red, Color::Green,
    Color::Yellow, Color::Blue, Color::Magenta, Color::Cyan, Color::White
};

const std::vector<Style> allStyles = {
    Style::Default, Style::Bold, Style::Italic, Style::Underline, Style::Reversed
};

void setColor(Color color = Color::Default, const std::vector<Style>& styles = {}, const std::string& font = "");
std::string colorToString(Color color);

extern Color chosenColor;
Color stringToColor(const std::string& str);