#include <fstream>
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

    uint8_t flags;
};

SaveFile deserialize(std::string path) {

}

void serialize(SaveFile save, std::string path) {
    std::ofstream file(path);

    file.write(reinterpret_cast<char*>(save.magic), sizeof(uint64_t));

    file.write(reinterpret_cast<char*>(save.versionHigh), sizeof(uint8_t));
    file.write(reinterpret_cast<char*>(save.versionMid), sizeof(uint8_t));
    file.write(reinterpret_cast<char*>(save.versionLow), sizeof(uint8_t));
    
    file.write(reinterpret_cast<char*>(save.historyLength), sizeof(uint32_t));

    file.write(reinterpret_cast<char*>(save.flags), sizeof(uint8_t));
    file.flush();
    file.close(); // close is supposed to save
}

int main(){
    SaveFile sf;
    sf.flags = 255; // all on
    sf.versionHigh = 1;
    sf.versionHigh = 0;
    sf.versionHigh = 0;
    // 1.0.0

    sf.historyLength = 0;

    serialize(sf, "hi.bc");
    return 0;
}