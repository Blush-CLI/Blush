#include <core/shell.hpp>

void handle_ctrlc(int) {}

void handle_segfault(int) {
    exit(-127);
}

int main() {
    signal(SIGINT, handle_ctrlc);
    signal(SIGSEGV, handle_segfault);

    Shell shell;
    shell.Init();
    return 0;
} // shortest main ever lmao