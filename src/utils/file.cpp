#include <utils/file.hpp>
#include <filesystem>
#include <print>
#include <sstream>
#include <fstream>

namespace File {
    /**
      * Reads the provided file
      * @param `std::string` filename
      * @returns `std::string`
      * @author x4raynixx
     */
    std::string readFile(const std::string_view filename) {
        std::ifstream file(filename.data());
        if (!file.is_open()) {
            std::println("Failed to open {}", filename.data());
            return "";
        }
        std::stringstream ss;
        ss << file.rdbuf();
        return ss.str(); // 
    }
    
    /**
      * Get last current working directory entry (eg, it will get Projects from /home/sky/Projects)
      * @returns `std::string`
      * @author landingpad75
     */ // what's lcwd : 
    std::string lcwd() {
        auto currentPath = std::filesystem::current_path(); // eg C:\Users\Raynixx\Documents\Codes\blush_revived\build
        return currentPath.filename().string(); // currentPath.filename() = build; .string turns it to a string
    }
}