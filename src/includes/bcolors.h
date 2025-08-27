#pragma once
#include <string>
#include <vector>

enum class Color { Default, Black, Red, Green, Yellow, Blue, Magenta, Cyan, White };
enum class Style { Bold, Italic, Underline, Reversed };
const std::vector<Color> allColors = {
    Color::Default, Color::Black, Color::Red, Color::Green,
    Color::Yellow, Color::Blue, Color::Magenta, Color::Cyan, Color::White
};

void setColor(Color color = Color::Default, const std::vector<Style>& styles = {}, const std::string& font = "");

extern Color chosenColor;