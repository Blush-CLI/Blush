#include <core/command.hpp>
#include <utils/string.hpp>
#include <span>

/**
 * Takes the first element of a vector and removes it from that said vector
 * */
template<typename T>
T takeFirst(std::span<T>& s){
    /*
        Move the first element of the vector (std::span) into this location
    */
    T value = std::move(s.front()); // splitUp[0] is now nullptr
    /*
        Create a new span without changing the vector removing the first element
    */
    s = s.subspan(1);
    return value;
}

Command::Command(const std::string& command){
    // split spaces (echo hello world => ["echo", "hello", "world"])
    auto splitUp = String::splitSpaces(command);
    
    std::span<std::string> s{splitUp};
    std::string commandName = takeFirst(s);

    this->arguments = splitUp;
    this->mainCommand = commandName;
}