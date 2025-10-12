#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <thread>
#include <chrono>
#include "../includes/loanim.h"
#include "../includes/bcolors.h"
#include <vector>

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#else
#include <unistd.h>
#include <pwd.h>
#endif

inline std::string getHomeDirectory() {
#ifdef _WIN32
    char path[MAX_PATH];
    if(SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, path))) {
        return std::string(path);
    }
    return "";
#else
    const char* home = getenv("HOME");
    if(home) return std::string(home);
    struct passwd* pw = getpwuid(getuid());
    if(pw) return std::string(pw->pw_dir);
    return "";
#endif
}

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
#ifdef _WIN32
    char tempPath[MAX_PATH];
    GetTempPathA(MAX_PATH, tempPath);
    std::string tmpZip = std::string(tempPath) + "blush-update.zip";
    std::string url = "https://cdn.getblush.xyz/latest/blush-win-x64.zip";
    std::string homeDir = getHomeDirectory();
    std::string targetDir = homeDir + "\\.blush\\";

    setColor(Color::Cyan);
    _la_animgo("Preparing update");
    _la_anim_edit("Downloading update");
    int res = system(("curl -s -L -o \"" + tmpZip + "\" \"" + url + "\"").c_str());
    if(res != 0) {
        _la_anim_edit("Download failed!");
        std::this_thread::sleep_for(std::chrono::seconds(2));
        _la_animstop();
        setColor(Color::Default);
        return;
    }

    std::ifstream f(tmpZip, std::ios::binary | std::ios::ate);
    auto size = f.tellg();
    f.close();
    if(size < 512 * 1024) {
        _la_anim_edit("Update file not ready!");
        std::this_thread::sleep_for(std::chrono::seconds(2));
        _la_animstop();
        setColor(Color::Default);
        std::filesystem::remove(tmpZip);
        return;
    }

    _la_animstop();
    setColor(Color::Green);
    std::cout << "Successfully downloaded update!" << std::endl;
    setColor(Color::Default);
    std::this_thread::sleep_for(std::chrono::milliseconds(800));
    setColor(Color::Yellow);
    system(("powershell -Command \"Expand-Archive -Force '" + tmpZip + "' '" + targetDir + "'\"").c_str());
    system(("start \"\" \"" + targetDir + "blush.exe\"").c_str());
    setColor(Color::Default);
    exit(0);

#else
    std::string tmpZip = "/tmp/blush-update.zip";
    std::string url = "https://cdn.getblush.xyz/latest/blush-linux-x64.zip";
    std::string homeDir = getHomeDirectory();
    std::string targetDir = homeDir + "/.blush/";

    setColor(Color::Cyan);
    _la_animgo("Preparing update");
    _la_anim_edit("Downloading update");
    int res = system(("curl -s -L -o \"" + tmpZip + "\" \"" + url + "\"").c_str());
    if(res != 0) {
        _la_anim_edit("Download failed!");
        std::this_thread::sleep_for(std::chrono::seconds(2));
        _la_animstop();
        setColor(Color::Default);
        return;
    }

    std::ifstream f(tmpZip, std::ios::binary | std::ios::ate);
    auto size = f.tellg();
    f.close();
    if(size < 512 * 1024) {
        _la_anim_edit("File not found on CDN!");
        std::this_thread::sleep_for(std::chrono::seconds(3));
        _la_animstop();
        setColor(Color::Default);
        std::filesystem::remove(tmpZip);
        return;
    }

    _la_animstop();
    setColor(Color::Green);
    std::cout << "Successfully downloaded update!" << std::endl;
    setColor(Color::Default);
    std::this_thread::sleep_for(std::chrono::milliseconds(800));
    setColor(Color::Yellow);
    system(("unzip -o \"" + tmpZip + "\" -d \"" + targetDir + "\"").c_str());
    system(("chmod +x \"" + targetDir + "blush\"").c_str());
    system(("exec \"" + targetDir + "blush\"").c_str());
    setColor(Color::Default);
    exit(0);
#endif
}
