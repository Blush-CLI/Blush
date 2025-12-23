#include <commands/base.hpp>
#include <core/history.hpp>
#include <utils/saveload.hpp>
#include <tui/colors.hpp>
#include <print>

namespace Commands {
    int save(Command command) {
        History& hist = History::getInstance();
        uint8_t flags = static_cast<uint8_t>((uint8_t)SaveFlags::Healthy | (uint8_t)SaveFlags::SaveHistory);
        
        if (SaveLoad::save(hist.getCommands(), flags)) {
            std::println("{}[  OK  ]{} Settings and history saved to {}{}",
                Color::Green, 
                Color::Purple,
                SaveLoad::getSaveFilePath(),
                Color::Reset);
            return 0;
        } else {
            std::println("{}[ERROR]{} Failed to save settings{}",
                Color::Red, 
                Color::Purple,
                Color::Reset);
            return 1;
        }
    }
}