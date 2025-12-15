#include <core/shell.hpp>
#include <utils/file.hpp>
#include <iostream>
#include <queue>
#include <print>

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
    createRunnerThread();
    run();
}

void Shell::run() {
    while(true){
        std::print("[{}] > ", File::lcwd());
        std::string command;

        while(!std::getline(std::cin, command));

        try {
            if(command.empty()) continue;
            commandQueue.push(command);
        } catch (std::exception& e) {
            std::println("An error happened: {}", e.what());
        }
    }
}

void Shell::createRunnerThread() {
    thread = std::jthread(runnerThreadCode);
}

Shell::~Shell() {
    std::println("Goodbye!");
}