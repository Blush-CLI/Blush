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
        return false;
    } else if (pid == 0) {
        std::vector<char*> argv;
        for (auto& arg : command.arguments) {
            argv.push_back(arg.data());
        }
        argv.push_back(nullptr); // ull pointer for end of array

        execvp(argv[0], argv.data());
        exit(127);
    } else {
        int status;
        if (waitpid(pid, &status, 0) == -1) { // process didn't exit successfully
            return false;
        }

        if (WIFEXITED(status)) { // back
            exitCode = WEXITSTATUS(status);
            return exitCode == 0; // did it exit successfully?
        } else {
            exitCode = -1;
            return false;
        }
    }
}