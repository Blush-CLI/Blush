#pragma once
#include <string>
#include "command.hpp"

class Job {
public:
    Job(Command cmd);

    bool run();
    int exitCode;
private:
    Command command;
    void* data; // this will be for multi platform data
};