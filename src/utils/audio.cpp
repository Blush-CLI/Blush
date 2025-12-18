#include <audio/audio.hpp>
#include <cstdio>
#include <cstdlib>

Audio::Audio() {
    ma_result result = ma_engine_init(nullptr, &engine);
    if (result != MA_SUCCESS) {
        std::printf("Audio engine init failed: %d\n", result);
        std::exit(1);
    }
}

void Audio::play(const std::string& filepath) {
    ma_result result = ma_engine_play_sound(&engine, filepath.c_str(), nullptr);
    if (result != MA_SUCCESS) {
        std::printf("Failed to play '%s': %d\n", filepath.c_str(), result);
    }
}

Audio::~Audio() {
    ma_engine_uninit(&engine);
}

Audio audio;