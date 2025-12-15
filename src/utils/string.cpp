#include <utils/string.hpp>
#include <sstream>

namespace String {
    std::vector<std::string> splitSpaces(const std::string& input){
        /*
            Turn the input into a string stream
        */
        std::istringstream str(input);
        std::vector<std::string> result;
        std::string word;

        /*
            Send current word into the word variable
            (Hello world):
                1st while iteration: word = Hello
                2nd while iteration: word = world
        */
        while(str >> word) {
            result.push_back(word);
        }

        /*
            return final vector
        */
        return result;
    }
} // namespace String