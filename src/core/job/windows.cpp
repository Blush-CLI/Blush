#include <core/job.hpp>
#include <windows.h>

#include <filesystem>
#include <sstream>

Job::Job(Command command) : command(command), exitCode(-1), data(nullptr) {}


bool Job::run(){
    STARTUPINFOA info{};
    info.cb = sizeof(info);

    PROCESS_INFORMATION pInfo = {};
    auto str = std::filesystem::current_path().string().c_str();

    if(!CreateProcessA(
        nullptr,
        const_cast<char*>(command.fullCommand.c_str()),
        nullptr,
        nullptr,
        false,
        0,
        nullptr,
        nullptr,
        &info,
        &pInfo
    )) return false; // failed

    WaitForSingleObject(pInfo.hProcess, INFINITE);

    DWORD code;
    
    GetExitCodeProcess(pInfo.hProcess, &code);
    exitCode = static_cast<int>(code);

    CloseHandle(pInfo.hProcess);
    CloseHandle(pInfo.hThread);

    return exitCode == 0;
}