#include "../includes/bcolors.h"
#include "../includes/systeminfo.h"
#include "../includes/loanim.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>
#include <thread>
#include <chrono>
#include <array>

#ifdef _WIN32
#include <windows.h>
#include <sysinfoapi.h>
#else
#include <unistd.h>
#endif

std::string execCommand(const std::string& cmd)
{
#ifndef _WIN32
    std::array<char, 128> buffer;
    std::string result;

    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe)
    {
        return "N/A";
    }

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
    {
        result += buffer.data();
    }

    pclose(pipe);

    if (!result.empty() && result.back() == '\n')
    {
        result.pop_back();
    }

    if (result.empty())
    {
        return "N/A";
    }

    return result;
#else
    return "N/A";
#endif
}

size_t visualLength(const std::string& str)
{
    size_t len = 0;

    for (size_t i = 0; i < str.size(); i++)
    {
        unsigned char c = str[i];

        if (c >= 0xF0)
        {
            len++;
            i += 3;
        }
        else if (c >= 0xE0)
        {
            len++;
            i += 2;
        }
        else if (c >= 0xC0)
        {
            len++;
            i += 1;
        }
        else
        {
            len++;
        }
    }

    return len;
}

std::string getRAMInfo()
{
#ifdef _WIN32
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);

    if (GlobalMemoryStatusEx(&status))
    {
        double totalGB = status.ullTotalPhys / 1024.0 / 1024.0 / 1024.0;
        double usedGB  = (status.ullTotalPhys - status.ullAvailPhys) / 1024.0 / 1024.0 / 1024.0;
        double freeGB  = status.ullAvailPhys / 1024.0 / 1024.0 / 1024.0;

        std::ostringstream out;
        out.precision(2);
        out << std::fixed
            << totalGB << " GiB total / "
            << usedGB  << " GiB used / "
            << freeGB  << " GiB free";

        return out.str();
    }

    return "N/A";

#else
    std::ifstream meminfo("/proc/meminfo");

    if (!meminfo.is_open())
    {
        return "N/A";
    }

    size_t total_kb  = 0;
    size_t avail_kb  = 0;
    std::string line;

    while (std::getline(meminfo, line))
    {
        std::istringstream iss(line);
        std::string key;
        size_t value;
        std::string unit;
        iss >> key >> value >> unit;

        if (key == "MemTotal:")
        {
            total_kb = value;
        }
        else if (key == "MemAvailable:")
        {
            avail_kb = value;
        }
    }

    size_t used_kb = total_kb - avail_kb;

    auto kbToGB = [](size_t kb) -> double
    {
        return kb / 1024.0 / 1024.0;
    };

    std::ostringstream out;
    out.precision(2);
    out << std::fixed
        << kbToGB(total_kb) << " GiB total / "
        << kbToGB(used_kb) << " GiB used / "
        << kbToGB(avail_kb) << " GiB free";

    return out.str();
#endif
}

std::string getCPUInfo()
{
#ifdef _WIN32
    return execCommand("wmic cpu get name 2>nul");
#else
    std::string cpu = execCommand("grep -m1 'model name' /proc/cpuinfo | awk -F: '{gsub(/^ +| +$/,\"\",$2); print $2}'");
    if (cpu.empty())
    {
        return "N/A";
    }
    return cpu;
#endif
}

std::string getDiskInfo()
{
#ifdef _WIN32
    return "C:?"; // fallback
#else
    return execCommand("df -h / | awk 'NR==2 {print $4}'");
#endif
}

void c_sinfo(const std::vector<std::string>& args)
{
    setColor(chosenColor);
    _la_animgo("Fetching");

    bool isWin = false;
#ifdef _WIN32
    isWin = true;
#endif

    std::string system = isWin ? execCommand("ver") : execCommand("uname -s") + " " + execCommand("uname -r");
    std::string disk   = getDiskInfo();
    std::string cpu    = getCPUInfo();
    std::string ram    = getRAMInfo();

    std::string e_sys  = isWin ? "[SYS]" : " 💻 ";
    std::string e_disk = isWin ? "[DSK]" : " 💾 ";
    std::string e_cpu  = isWin ? "[CPU]" : " 🧠 ";
    std::string e_ram  = isWin ? "[RAM]" : " 🚀 ";

    std::vector<std::tuple<std::string, std::string, std::string>> infos;
    infos.push_back(std::make_tuple(e_sys,  "System: ",      system));
    infos.push_back(std::make_tuple(e_disk, "Current Disk: ", disk));
    infos.push_back(std::make_tuple(e_cpu,  "CPU: ",         cpu));
    infos.push_back(std::make_tuple(e_ram,  "RAM: ",         ram));

    // Compute max length
    size_t maxLength = 0;
    for (auto& t : infos)
    {
        auto& [e, l, v] = t;
        size_t len = visualLength("  " + e + (isWin ? "|" : "│") + " " + l + v + " ");
        if (len > maxLength)
        {
            maxLength = len;
        }
    }

    // Print top border
    setColor(chosenColor);
    _la_animstop();
    std::cout << "+";
    for (size_t i = 0; i < maxLength + 3; i++)
    {
        std::cout << "-";
    }
    std::cout << "+\n";

    // Print info lines
    for (auto& t : infos)
    {
        auto& [e, l, v] = t;
        std::string sep = isWin ? "|" : "│";
        std::cout << sep << "  " << e << sep << " " << l << v;

        size_t visLen = visualLength("  " + e + sep + " " + l + v + " ");
        if (visLen < maxLength + 3)
        {
            for (size_t i = visLen; i < maxLength + 3; i++)
            {
                std::cout << " ";
            }
        }

        std::cout << sep << "\n";
    }

    // Print bottom border
    std::cout << "+";
    for (size_t i = 0; i < maxLength + 3; i++)
    {
        std::cout << "-";
    }
    std::cout << "+\n";
    setColor(); // reset
}
