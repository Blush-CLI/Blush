#pragma once
#include <iostream>
#include <functional>
#include <map>
#include <string>
#include <vector>
#include "./bcolors.h"
#include "./updater.h"

using CommandFunc = std::function<void(const std::vector<std::string>&)>;

struct Command {
    CommandFunc func;
    std::string description;
};

void c_cls(const std::vector<std::string>& args = {});
void c_help(const std::vector<std::string>& args = {});
void c_quit(const std::vector<std::string>& args = {});
void c_color(const std::vector<std::string>& args = {});
void c_history(const std::vector<std::string>& args = {});
void c_sinfo(const std::vector<std::string>& args = {});
void c_version(const std::vector<std::string>& args = {});

inline std::map<std::string, Command> commands = {
    {"cls",     {c_cls,     "Clears the terminal screen"}},
    {"clear",   {c_cls,     "Clears the terminal screen"}},
    {"help",    {c_help,    "Displays all built-in commands"}},
    {"quit",    {c_quit,    "Exits Blush"}},
    {"q",       {c_quit,    "Exits Blush"}},
    {"exit",    {c_quit,    "Exits Blush"}},
    {"color",   {c_color,   "Selects or lists available text colors"}},
    {"history", {c_history, "Displays command history"}},
    {"sinfo",   {c_sinfo,   "Shows system information (CPU, RAM, Disk, OS)"}},
    {"version", {c_version, "Shows current blush version"}},
    {"update",  {selfUpdate, "Updates blush to the latest version"}},
};