#pragma once
#include <iostream>
#include <string>
#include <strings.h>
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <thread>
#include <chrono>
#include <algorithm>
#include "../includes/loanim.h"
#include "../includes/bcolors.h"
#include "../includes/blversion.h"
#include "../includes/cfm.h"

// This mightnot work properly on all platforms

#ifdef _WIN32
#include <windows.h>
#endif

inline std::string getExecutablePath() {
#ifdef _WIN32
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    return std::string(buffer);
#else
    char buffer[4096];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer)-1);
    if(len != -1) {
        buffer[len] = '\0';
        return std::string(buffer);
    }
    return "";
#endif
}

inline void selfUpdate(const std::vector<std::string>& args) {
    _la_animgo("Fetching newest online version");
    std::string resNV = cfm::fetch("https://cdn.getblush.xyz/newestv.txt");
    _la_animstop();

    if (resNV == "error code: 1015") {
        setColor(Color::Red);
        std::cout << "[Error] Too many requests!\n";
        setColor(chosenColor);
        return;
    }

    if (resNV.empty() || resNV == "Not Found" || resNV == "File not found") {
        setColor(Color::Red);
        if (resNV.empty())
            std::cout << "[Error] Couldn't check newest version online, blank response\n";
        else if (resNV == "Not Found")
            std::cout << "[Error] Endpoint '/get' failed to resolve\nYour Blush is outdated, please update\n";
        else
            std::cout << "[Error] Newest version file doesn't exist anymore\nIt might be that your Blush is outdated, or it's a server problem\n";
        setColor(chosenColor);
        return;
    }

    if (resNV == blversion) {
        setColor(Color::White);
        std::cout << "----------------------\n";
        setColor(Color::Green);
        std::cout << "Everything up to date\n";
        setColor(Color::White);
        std::cout << "----------------------\n";
        setColor(chosenColor);
        return;
    }

#ifdef _WIN32
    std::string url = "https://cdn.getblush.xyz/blush-win-x64.exe";
    std::string tmpFile = "blush-new.exe";
    std::string exePath = getExecutablePath();

    setColor(Color::Cyan);
    _la_animgo("Preparing update");
    _la_anim_edit("Downloading update");
    int res = system(("curl -s -L -o \"" + tmpFile + "\" \"" + url + "\"").c_str());
    if(res != 0) {
        _la_anim_edit("Download failed!");
        std::this_thread::sleep_for(std::chrono::seconds(2));
        _la_animstop();
        setColor(Color::Default);
        return;
    }

    std::ifstream f(tmpFile, std::ios::binary | std::ios::ate);
    auto size = f.tellg();
    f.close();
    if(size < 0.5 * 1024 * 1024) {
        _la_anim_edit("Update file not ready!");
        std::this_thread::sleep_for(std::chrono::seconds(2));
        _la_animstop();
        setColor(Color::Default);
        std::filesystem::remove(tmpFile);
        return;
    }

    _la_animstop();
    setColor(Color::Green);
    std::cout << "Successfully downloaded update!" << std::endl;
    setColor(Color::Default);
    std::this_thread::sleep_for(std::chrono::milliseconds(800));
    setColor(Color::Yellow);
    std::string script = "ping 127.0.0.1 -n 2 > nul & del \"" + exePath + "\" & rename \"" + tmpFile + "\" \"" + exePath + "\" & start \"\" \"" + exePath + "\"";
    system(("cmd /C \"" + script + "\"").c_str());
    setColor(Color::Default);
    exit(0);

#else
    std::string url = "https://cdn.getblush.xyz/latest/blush-linux-x64";
    std::string tmpFile = "blush-new";
    std::string exePath = getExecutablePath();

    setColor(Color::Cyan);
    _la_animgo("Preparing update");
    _la_anim_edit("Downloading update");
    int res = system(("curl -s -L -o \"" + tmpFile + "\" \"" + url + "\"").c_str());
    if(res != 0) {
        _la_anim_edit("Download failed!");
        std::this_thread::sleep_for(std::chrono::seconds(2));
        _la_animstop();
        setColor(Color::Default);
        return;
    }

    std::ifstream f(tmpFile, std::ios::binary | std::ios::ate);
    auto size = f.tellg();
    f.close();
    if(size < 0.5 * 1024 * 1024) {
        _la_anim_edit("File not found on CDN!");
        std::this_thread::sleep_for(std::chrono::seconds(3));
        _la_animstop();
        setColor(Color::Default);
        std::filesystem::remove(tmpFile);
        return;
    }

    _la_animstop();
    setColor(Color::Green);
    std::cout << "Successfully downloaded update!" << std::endl;
    setColor(Color::Default);
    std::this_thread::sleep_for(std::chrono::milliseconds(800));
    setColor(Color::Yellow);
    system(("chmod +x \"" + tmpFile + "\"").c_str());
    system(("rm \"" + exePath + "\"").c_str());
    system(("mv \"" + tmpFile + "\" \"" + exePath + "\"").c_str());
    system(("exec \"" + exePath + "\"").c_str());
    setColor(Color::Default);
    exit(0);
#endif
}
