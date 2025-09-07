/*
 File: src/modules/history.cpp
 Created: [03.08.2025]
 Description: Manages history and all assiocated things with it
*/

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include "../includes/iojson.h"
#include "../includes/historyhandler.h"
#include "../includes/bcolors.h"

void addHistory(const std::string& content) {
    nlohmann::json j = getConfigValueFromFile("blush_dir", "history{}", "entries");
    if (!j.is_array()) j = nlohmann::json::array();

    j.insert(j.begin(), content);

    while (j.size() > 100) {
        j.erase(j.end() - 1);
    }

    saveConfigToFilePath("blush_dir", "history{}", "entries", j);
}

std::vector<std::string> getHistory() {
    nlohmann::json j = getConfigValueFromFile("blush_dir", "history{}", "entries");
    std::vector<std::string> history;

    if (j.is_array()) {
        for (auto& el : j) {
            if (el.is_string()) history.push_back(el.get<std::string>());
        }
    }

    return history;
}

void printHistory() {
    std::vector<std::string> history = getHistory();
    int i_historyp = 0;
    for (const auto& cmd : history) {
        i_historyp+=1;
        setColor(Color::Blue);
        std::cout << "  > " << std::setfill('0') << std::setw(2) << i_historyp << ": ";
        setColor(chosenColor);
        std::cout << cmd << std::endl;
    }
    setColor(Color::Default);
}

void resetHistory() {
    saveConfigToFilePath("blush_dir", "history{}", "entries", nlohmann::json::array());
}
