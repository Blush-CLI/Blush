#include <commands/base.hpp>
#include <core/shell.hpp>
#include <core/job.hpp>
#include <utils/file.hpp>
#include <utils/input.hpp>
#include <tui/colors.hpp>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <print>
#include <blush.hpp>

Shell::Shell() { // Colors:
    std::println("{}[  OK  ]{} Shell initialized!{}",
        Color::Green, 
        Color::Purple,
        Color::Reset); // systemd style fr
    run();
}

void Shell::run() {
    Input input;
    while(true){
        std::print("{}[{}]{} {}> {}",Color::Purple, File::lcwd(), Color::Purple, Color::Blue, Color::Reset);
        std::string command;
        while(char c = std::getchar()){
            switch(c + 'A' - 1){
                case 'Q':
                    exit(0);
            }
            if(c == '\n') break;
            command += c;
        }
        try {
            if(command.empty()) continue; // if command is empty restart the loop
            
            Command cmd(command);

            if(cmd.mainCommand == "exit") exit(0);
            
            if(map.find(cmd.mainCommand) != map.end()) {
                map[cmd.mainCommand].func(cmd);
            } else {
                Job job(command);
                bool result = job.run();
                if(!result) {
                    std::println("Error: Command not found.\n Exit code: {}", job.exitCode); // hmm
                } // alt up arrow
            }
        } catch (std::exception& e) {
            std::println("An error occured: {}", e.what());
        }
    }
}

Shell::~Shell() {
    std::println("Goodbye!");
}