#include <commands/base.hpp>
#include <core/shell.hpp>
#include <core/job.hpp>
#include <core/history.hpp>
#include <utils/file.hpp>
#include <utils/input.hpp>
#include <utils/saveload.hpp>
#include <tui/colors.hpp>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <print>
#include <blush.hpp>
extern Audio audio;

void Shell::Init() {
    this->audio = &::audio;

    std::vector<std::string> savedHistory;
    uint8_t flags = 0;
    if (SaveLoad::load(savedHistory, flags)) {
        if (flags & static_cast<uint8_t>(SaveFlags::SaveHistory)) {
            History& hist = History::getInstance();
            for (const auto& command : savedHistory) {
                hist.add(command);
            }
            std::println("{}[  OK  ]{} Loaded {} commands from history{}",
                Color::Green, 
                Color::Purple,
                savedHistory.size(),
                Color::Reset);
        }
    }
    
    std::println("{}[  OK  ]{} Shell initialized!{}",
        Color::Green, 
        Color::Purple,
        Color::Reset);
    int rcode = run();
    D_PRINTLN("Shell process exited with code {}", rcode);
}

void printPrompt() {
    std::print("{}[{}] {}> {}",
        Color::Purple,
        File::lcwd(),
        Color::Blue,
        Color::Reset
    );
}

int Shell::run() {
    while(true){
        std::string command = Input::input();
        
        try {
            if(command.empty()) continue;
            Command cmd(command);
            
            if(commands_map.find(cmd.mainCommand) != commands_map.end()) {
                int exitCode = commands_map[cmd.mainCommand].func(cmd);
                setenv("?", std::to_string(exitCode).c_str(), 1);
            } else {
                Job job(cmd);
                bool result = job.run();
                if(job.exitCode == 127) {
                    std::println("{}Error: '{}{}{}{}' | Command not found. \
                        \n{}> Exit code: {}",
                        Color::Red, Color::Bold, cmd.mainCommand, Color::Reset, Color::Red, Color::Yellow, job.exitCode);
                }
                setenv("?", std::to_string(job.exitCode).c_str(), 1);
            }
        } catch (std::exception& e) {
            std::println("An error occured: {}", e.what());
        }
    }
    return 0;
}

Shell::~Shell() {
    History& hist = History::getInstance();
    uint8_t flags = static_cast<uint8_t>((uint8_t)SaveFlags::Healthy | (uint8_t)SaveFlags::SaveHistory);
    
    if (SaveLoad::save(hist.getCommands(), flags)) {
        std::println("{}[  OK  ]{} History saved{}",
            Color::Green, 
            Color::Purple,
            Color::Reset);
    } else {
        std::println("{}[WARN]{} Failed to save history{}",
            Color::Yellow, 
            Color::Purple,
            Color::Reset);
    }
    
    std::println("Goodbye!");
}