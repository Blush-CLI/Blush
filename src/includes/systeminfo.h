#pragma once
#include <string>
#include <vector>

std::string execCommand(const std::string& cmd);
void loadingAnimation();
size_t visualLength(const std::string& str);
std::string getRAMInfo();
std::string getCPUInfo();
std::string getDiskInfo();
void c_sinfo(const std::vector<std::string>& args={});
