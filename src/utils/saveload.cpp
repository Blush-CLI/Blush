#include "utils/saveload.hpp"
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <cstring>
#include <iostream>

std::string SaveLoad::getSaveFilePath() {
    #ifdef _WIN32
        const char* home = std::getenv("USERPROFILE");
    #else
        const char* home = std::getenv("HOME");
    #endif
    if (!home) {
        return "";
    }
    
    std::filesystem::path savePath = std::filesystem::path(home) / ".blush" / "save.blsh";
    return savePath.string();
}

bool SaveLoad::createSaveDirectory() {
    #ifdef _WIN32
        const char* home = std::getenv("USERPROFILE");
    #else
        const char* home = std::getenv("HOME");
    #endif
    if (!home) {
        return false;
    }
    
    std::filesystem::path blushDir = std::filesystem::path(home) / ".blush";
    
    try {
        if (!std::filesystem::exists(blushDir)) {
            std::filesystem::create_directories(blushDir);
        }
        return true;
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Failed to create save directory: " << e.what() << std::endl;
        return false;
    }
}

bool SaveLoad::save(const std::vector<std::string>& history, uint8_t flags) {
    if (!createSaveDirectory()) {
        return false;
    }
    
    std::string savePath = getSaveFilePath();
    if (savePath.empty()) {
        return false;
    }
    
    std::ofstream file(savePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open save file for writing: " << savePath << std::endl;
        return false;
    }
    
    uint64_t magic = 0xdeadcafe;
    file.write(reinterpret_cast<const char*>(&magic), sizeof(magic));
    
    file.write(reinterpret_cast<const char*>(&VERSION_HIGH), sizeof(VERSION_HIGH));
    file.write(reinterpret_cast<const char*>(&VERSION_MID), sizeof(VERSION_MID));
    file.write(reinterpret_cast<const char*>(&VERSION_LOW), sizeof(VERSION_LOW));
    
    uint32_t historyLength = static_cast<uint32_t>(history.size());
    file.write(reinterpret_cast<const char*>(&historyLength), sizeof(historyLength));
    
    for (const auto& command : history) {
        uint32_t commandLength = static_cast<uint32_t>(command.length());
        file.write(reinterpret_cast<const char*>(&commandLength), sizeof(commandLength));
        file.write(command.c_str(), commandLength);
    }
    
    file.write(reinterpret_cast<const char*>(&flags), sizeof(flags));
    
    file.close();
    return true;
}

bool SaveLoad::load(std::vector<std::string>& history, uint8_t& flags) {
    std::string savePath = getSaveFilePath();
    if (savePath.empty()) {
        return false;
    }
    
    std::ifstream file(savePath, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    uint64_t magic;
    file.read(reinterpret_cast<char*>(&magic), sizeof(magic));
    if (file.gcount() != sizeof(magic) || magic != 0xdeadcafe) {
        std::cerr << "Invalid save file format (bad magic number)" << std::endl;
        file.close();
        return false;
    }
    
    uint8_t versionHigh, versionMid, versionLow;
    file.read(reinterpret_cast<char*>(&versionHigh), sizeof(versionHigh));
    file.read(reinterpret_cast<char*>(&versionMid), sizeof(versionMid));
    file.read(reinterpret_cast<char*>(&versionLow), sizeof(versionLow));
    
    if (file.gcount() != sizeof(versionLow)) {
        std::cerr << "Invalid save file format (incomplete version)" << std::endl;
        file.close();
        return false;
    }
    
    uint32_t historyLength;
    file.read(reinterpret_cast<char*>(&historyLength), sizeof(historyLength));
    if (file.gcount() != sizeof(historyLength)) {
        std::cerr << "Invalid save file format (incomplete history length)" << std::endl;
        file.close();
        return false;
    }
    
    history.clear();
    history.reserve(historyLength);
    
    for (uint32_t i = 0; i < historyLength; ++i) {
        uint32_t commandLength;
        file.read(reinterpret_cast<char*>(&commandLength), sizeof(commandLength));
        if (file.gcount() != sizeof(commandLength)) {
            std::cerr << "Invalid save file format (incomplete command length)" << std::endl;
            file.close();
            return false;
        }
        
        std::string command(commandLength, '\0');
        file.read(&command[0], commandLength);
        if (file.gcount() != static_cast<std::streamsize>(commandLength)) {
            std::cerr << "Invalid save file format (incomplete command data)" << std::endl;
            file.close();
            return false;
        }
        
        history.push_back(std::move(command));
    }
    
    file.read(reinterpret_cast<char*>(&flags), sizeof(flags));
    if (file.gcount() != sizeof(flags)) {
        std::cerr << "Invalid save file format (incomplete flags)" << std::endl;
        file.close();
        return false;
    }
    
    file.close();
    return true;
}