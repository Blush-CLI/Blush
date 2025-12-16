#include <core/history.hpp>

void History::add(const std::string& command) {
    if(command.empty()) return;
    
    if(!commands.empty() && commands.back() == command) return;
    
    commands.push_back(command);
    currentIndex = commands.size();
}

std::string History::getPrevious() {
    if(commands.empty()) return "";
    
    if(currentIndex > 0) {
        currentIndex--;
    }
    
    if(currentIndex >= 0 && currentIndex < commands.size()) {
        return commands[currentIndex];
    }
    
    return "";
}

std::string History::getNext() {
    if(commands.empty()) return "";
    
    if(currentIndex < commands.size()) {
        currentIndex++;
    }
    
    if(currentIndex < commands.size()) {
        return commands[currentIndex];
    }
    
    currentIndex = commands.size();
    return "";
}

void History::reset() {
    currentIndex = commands.size();
}

bool History::isEmpty() const {
    return commands.empty();
}

size_t History::size() const {
    return commands.size();
}

void History::clear() {
    commands.clear();
    currentIndex = -1;
}

const std::vector<std::string>& History::getCommands() const {
    return commands;
}