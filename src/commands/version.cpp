#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <string>
#include "../includes/bcolors.h"
#include "../includes/blversion.h"
#include "../includes/loanim.h"
#include "../includes/cfm.h"

void c_version(const std::vector<std::string>& args) {
    _la_animgo("Fetching newest online version");
    std::string res = cfm::fetch("https://cdn.getblush.xyz/get?file=newestv.txt");
    _la_animstop();

    if (res == "error code: 1015") {
        setColor(Color::Red);
        std::cout << "[Error] Too many requests!\n";
        setColor(chosenColor);
        return;
    }

    if (res.empty() || res == "Not Found" || res == "File not found") {
        setColor(Color::Red);
        if (res.empty())
            std::cout << "[Error] Couldn't check newest version online, blank response\n";
        else if (res == "Not Found")
            std::cout << "[Error] Endpoint '/get' failed to resolve\nYour Blush is outdated, please update\n";
        else
            std::cout << "[Error] Newest version file doesn't exist anymore\nIt might be that your Blush is outdated, or it's a server problem\n";
        setColor(chosenColor);
        return;
    }

    if (res == blversion) {
        setColor(Color::White);
        std::cout << "----------------------\n";
        setColor(Color::Magenta);
        std::cout << "Blush ";
        setColor(Color::Yellow);
        std::cout << "v" << blversion << "\n";
        setColor(Color::Green);
        std::cout << "Everything up to date\n";
        setColor(Color::White);
        std::cout << "----------------------\n";
        setColor(chosenColor);
        return;
    }

    int onlinevernumber = 0;
    int localvernumber = 0;
    try {
        std::string onlineStr = res;
        onlineStr.erase(std::remove(onlineStr.begin(), onlineStr.end(), '.'), onlineStr.end());
        onlinevernumber = std::stoi(onlineStr);

        std::string blushversion = blversion;
        blushversion.erase(std::remove(blushversion.begin(), blushversion.end(), '.'), blushversion.end());
        localvernumber = std::stoi(blushversion);
    } catch (...) {
        setColor(Color::Red);
        std::cout << "[Error] An Unknown error occured!\n";
        setColor(chosenColor);
        return;
    }

    if (onlinevernumber > localvernumber) {
        setColor(Color::White);
       std::cout << "-----------------------\n";
        setColor(Color::Magenta);
        std::cout << "Blush ";
        setColor(Color::Yellow);
        std::cout << "v" << blversion << "\n";
        setColor(Color::Red);
        std::cout << "Newest Version: ";
        setColor(Color::Yellow);
        std::cout << "v" << res << "\n";
        setColor(Color::White);
        std::cout << "-----------------------\n";
    } else if (onlinevernumber < localvernumber) {
        setColor(Color::Yellow);
        std::cout << "[Confusion] Your local version is ahead of the CDN version.\n"
             "To fix this issue, please run the 'update' command.\n";
    } else {
        setColor(Color::Green);
        std::cout << "Everything up to date\n";
    }

    setColor(chosenColor);
}
