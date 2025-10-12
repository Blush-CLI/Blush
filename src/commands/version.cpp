#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <string>
#include <fstream>
#include <filesystem>
#include <ctime>
#include "../includes/bcolors.h"
#include "../includes/blversion.h"
#include "../includes/loanim.h"
#include "../includes/cfm.h"

#ifdef _WIN32
#include <cstdlib>
#endif

namespace fs = std::filesystem;

std::string getBlushCertPath() {
#ifdef _WIN32
    char* envVar = std::getenv("LOCALAPPDATA");
    if (!envVar) return "";
    fs::path p = fs::path(envVar) / ".blush" / "cacert.pem";
    fs::create_directories(p.parent_path());
    return p.string();
#else
    return "";
#endif
}

std::string fetchCertIfNeeded() {
#ifdef _WIN32
    std::string certPath = getBlushCertPath();
    bool fetchNew = true;
    if (fs::exists(certPath)) {
        auto ftime = fs::last_write_time(certPath);
        auto sctp = std::chrono::file_clock::to_sys(ftime);
        std::time_t file_time = std::chrono::system_clock::to_time_t(sctp);
        std::time_t now = std::time(nullptr);
        if (now - file_time < 30 * 24 * 60 * 60) fetchNew = false;
    }
    if (fetchNew) {
        CURL* curl = curl_easy_init();
        if (curl) {
            std::string certData;
            curl_easy_setopt(curl, CURLOPT_URL, "https://curl.se/ca/cacert.pem");
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cfm::WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &certData);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
            CURLcode res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            if (res == CURLE_OK && !certData.empty()) {
                std::ofstream out(certPath, std::ios::binary);
                out << certData;
                out.close();
            }
        }
    }
    return certPath;
#else
    return "";
#endif
}

void c_version(const std::vector<std::string>& args) {
    _la_animgo("Fetching newest online version");
#ifdef _WIN32
    std::string certPath = fetchCertIfNeeded();
    std::string res = cfm::fetch("https://cdn.getblush.xyz/newestv.txt", certPath);
#else
    std::string res = cfm::fetch("https://cdn.getblush.xyz/newestv.txt");
#endif
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
        std::cout << "\n[Confusion] Your local version is ahead of the CDN version.\n"
                  << "To fix this issue, please run the 'update' command.\n";
    } else {
        setColor(Color::Green);
        std::cout << "Everything up to date\n";
    }

    setColor(chosenColor);
}
