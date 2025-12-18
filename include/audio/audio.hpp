#pragma once
#include <string>
#include <miniaudio.h>

class Audio {
public:
    Audio();
    ~Audio();

    void play(const std::string& filepath);

private:
    ma_engine engine;
};
