#include <string>

/**
 * removes whitespace from start and end of a string
 */
std::string trimWhitespace(const std::string& text) {
    size_t start = text.find_first_not_of(" \r\n\t");
    size_t end = text.find_last_not_of(" \r\n\t");
    
    if (start == std::string::npos) {
        return "";
    }
    
    return text.substr(start, end - start + 1);
}