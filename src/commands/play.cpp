#include <commands/base.hpp>
#include <audio/audio.hpp>
#include <blush.hpp>
#include <string>
#include <filesystem>
#include <print>

extern Audio audio;

namespace Commands {
    int play(Command command){
        auto fn = command.arguments[1];

        audio.play(fn);

        return 0;
    }
}