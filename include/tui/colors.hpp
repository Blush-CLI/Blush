#pragma once

#include <string>
#include <iostream>

class Color {
public:
    static constexpr std::string Black      = "\e[0;30m";
    static constexpr std::string Red        = "\e[0;31m";
    static constexpr std::string Green      = "\e[0;32m";
    static constexpr std::string Yellow     = "\e[0;33m";
    static constexpr std::string Blue       = "\e[0;34m";
    static constexpr std::string Purple     = "\e[0;35m";
    static constexpr std::string Cyan       = "\e[0;36m";
    static constexpr std::string White      = "\e[0;37m";
    static constexpr std::string Reset      = "\e[0m";
};