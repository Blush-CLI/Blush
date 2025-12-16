#pragma once

#include <string_view>

struct Color {
    static constexpr std::string_view Black  = "\x1b[0;30m";
    static constexpr std::string_view Red    = "\x1b[0;31m";
    static constexpr std::string_view Green  = "\x1b[0;32m";
    static constexpr std::string_view Yellow = "\x1b[0;33m";
    static constexpr std::string_view Blue   = "\x1b[0;34m";
    static constexpr std::string_view Purple = "\x1b[0;35m";
    static constexpr std::string_view Cyan   = "\x1b[0;36m";
    static constexpr std::string_view White  = "\x1b[0;37m";

    static constexpr std::string_view Bold   = "\x1b[1m";
    static constexpr std::string_view Reset  = "\x1b[0m";
};
