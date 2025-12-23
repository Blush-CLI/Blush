#include <core/job.hpp>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <sstream>

Job::Job(Command command) : command(command), exitCode(-1), data(nullptr) {}

bool Job::run() {
    pid_t pid = fork();
    if (pid < 0) {
        exitCode = -1;
        return false;
    } else if (pid == 0) {
        std::vector<char*> argv;
        argv.push_back(const_cast<char*>(command.mainCommand.c_str()));
        for (size_t i = 1; i < command.arguments.size(); i++) {
            argv.push_back(const_cast<char*>(command.arguments[i].c_str()));
        }
        argv.push_back(nullptr);

        execvp(argv[0], argv.data());
        _exit(127);
    } else {
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            exitCode = -1;
            return false;
        }

        if (WIFEXITED(status)) {
            exitCode = WEXITSTATUS(status);
            return exitCode == 0;
        } else {
            exitCode = -1;
            return false;
        }
    }
}