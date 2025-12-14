#pragma once

#include <string>
#include <string_view>

namespace File {
    std::string readFile(const std::string_view filename);
    std::string lcwd();
} // namespace File
