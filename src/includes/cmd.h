/*
 File: src/includes/cmd.h
 Created: [25.08.2025]
 Description: All built-in commands defined here
*/

#pragma once
#include <iostream>
#include <functional>
#include <map>
#include <string>
#include <vector>
#include "./bcolors.h"

void c_cls(const std::vector<std::string>& args = {});
void c_help(const std::vector<std::string>& args = {});
void c_quit(const std::vector<std::string>& args = {});
void c_color(const std::vector<std::string>& args = {});
void c_history(const std::vector<std::string>& args = {});
void c_sinfo(const std::vector<std::string>& args = {});

inline std::map<std::string, std::function<void(const std::vector<std::string>&)>> commands = {
    {"cls", c_cls},
    {"clear", c_cls},
    {"help", c_help},
    {"quit", c_quit},
    {"q", c_quit},
    {"exit", c_quit},
    {"color", c_color},
    {"history", c_history},
    {"sinfo", c_sinfo},
};