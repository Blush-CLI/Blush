#pragma once
#include <string>
#include <vector>

enum class Color { Default, Black, Red, Green, Yellow, Blue, Magenta, Cyan, White };
enum class Style { Bold, Italic, Underline, Reversed };

void setColor(Color color = Color::Default, const std::vector<Style>& styles = {}, const std::string& font = "");