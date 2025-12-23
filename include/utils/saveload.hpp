#pragma once
#include <cstdint>
#include <string>
#include <vector>

enum SaveFlags {
    Healthy = 1 << 0, // 1
    HasShellLanguageSupport = 1 << 1,
    SaveHistory = 1 << 2,
    // what config would we want to have for now, like just some true/false ones
    // in normal blush you had json config, this is binary config :pray: its smaller in size plus our own format
};

struct SaveFile {
    uint64_t magic = 0xdeadcafe;

    uint8_t versionHigh, versionMid, versionLow; // 1.0.0 like

    uint32_t historyLength;
    char** commands; // char** = array of char *

    uint8_t flags;
};

class SaveLoad {
public:
    static bool save(const std::vector<std::string>& history, uint8_t flags = static_cast<uint8_t>(SaveFlags::Healthy | SaveFlags::SaveHistory));
    static bool load(std::vector<std::string>& history, uint8_t& flags);
    static std::string getSaveFilePath();
    static bool createSaveDirectory();
    
private:
    static constexpr uint8_t VERSION_HIGH = 1;
    static constexpr uint8_t VERSION_MID = 0;
    static constexpr uint8_t VERSION_LOW = 0;
};