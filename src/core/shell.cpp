#include <commands/base.hpp>
#include <core/shell.hpp>
#include <core/job.hpp>
#include <utils/file.hpp>
#include <iostream>
#include <queue>
#include <print>
#include <blush.hpp>

std::queue<Command> commandQueue;
std::mutex commandExecution;

void runnerThreadCode() {
    while(true){
        std::lock_guard guard(commandExecution);
        if(commandQueue.empty()) continue;

        auto current = commandQueue.front();

        system(current.fullCommand.c_str());

        commandQueue.pop();
    }
}

Shell::Shell() {
    std::println("Welcome to Blush!");
    run();
}

void Shell::run() {
    while(true){
        std::print("[{}] > ", File::lcwd());
        std::string command;

        while(!std::getline(std::cin, command));

        try {
            if(command.empty()) continue; // if command is empty restart the loop
            
            D_PRINTLN("Running - Command: {}", command);
            Command cmd(command);
            
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

void Shell::createRunnerThread() {
    // thread = std::jthread(runnerThreadCode);
}

Shell::~Shell() {
    std::println("Goodbye!");
}