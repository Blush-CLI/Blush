#pragma once
#include <cstdint>

enum class SaveFlags {
    Healthy = 1 << 0, // 1
    HasShellLanguageSupport = 1 << 1, // 2
    // what config would we want to have for now, like just some true/false ones
    // in normal blush you had json config, this is binary config :pray: its smaller in size plus our own format
};

struct SaveFile { // we doing binary (scary frfr)
    uint64_t magic = 0xdeadcafe; // dead cafe fr

    uint8_t versionHigh, versionMid, versionLow; // 1.0.0 like

    uint32_t historyLength;
    char** commands; // char** = array of char *

    uint8_t flags;
};