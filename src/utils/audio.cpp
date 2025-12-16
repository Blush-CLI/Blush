#include <utils/audio.hpp>
#include <miniaudio.h>

ma_engine engine;

Audio::Audio(){
    ma_result result = ma_engine_init(nullptr, &engine);
    if(result != MA_SUCCESS){
        exit(0);
        return;
    }

}

void Audio::play(std::string filepath){
    ma_engine_play_sound(&engine, filepath.c_str(), nullptr);
}

Audio::~Audio(){
    ma_engine_uninit(&engine);
}