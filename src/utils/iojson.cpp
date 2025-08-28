#include <iostream>
#include <fstream>
#include <filesystem>
#include "./json.hpp"
#include "../includes/iojson.h"

using json = nlohmann::json;
namespace fs = std::filesystem;

#ifdef _WIN32
const std::string BASE_DIR = std::string(std::getenv("LOCALAPPDATA")) + "\\.blush";
#else
const std::string BASE_DIR = std::string(std::getenv("HOME")) + "/.blush";
#endif

const std::string CONFIG_FILE = BASE_DIR + "/config.json";

json loadConfig() {
    if (!fs::exists(BASE_DIR))
        fs::create_directories(BASE_DIR);
    if (!fs::exists(CONFIG_FILE)) {
        std::ofstream o(CONFIG_FILE);
        o << "{}";
        o.close();
    }
    std::ifstream i(CONFIG_FILE);
    json j;
    i >> j;
    return j;
}

void saveConfigToFile(const json& j) {
    std::ofstream o(CONFIG_FILE);
    o << j.dump(4);
}

void saveConfig(const std::string& path, const std::string& name, const json& value) {
    json j = loadConfig();
    json* target = &j;
    std::string key = path;
    bool isArray = false;

    if (!path.empty() && path.back() == '}') {
        key.pop_back();
        if (!key.empty() && key.back() == '{') key.pop_back();
    } else if (!path.empty() && path.back() == ']') {
        key.pop_back();
        if (!key.empty() && key.back() == '[') key.pop_back();
        isArray = true;
    }

    if (!target->contains(key)) {
        if (isArray) (*target)[key] = json::array();
        else (*target)[key] = json::object();
    }

    if (isArray) {
        json& arr = (*target)[key];
        bool found = false;
        for (auto& el : arr) {
            if (el.is_object() && el.contains(name)) {
                el[name] = value;
                found = true;
                break;
            }
        }
        if (!found) arr.push_back({{name, value}});
    } else {
        (*target)[key][name] = value;
    }

    saveConfigToFile(j);
}

void removeConfig(const std::string& path, const std::string& name) {
    json j = loadConfig();
    json* target = &j;
    std::string key = path;
    bool isArray = false;

    if (!path.empty() && path.back() == '}') {
        key.pop_back();
        if (!key.empty() && key.back() == '{') key.pop_back();
    } else if (!path.empty() && path.back() == ']') {
        key.pop_back();
        if (!key.empty() && key.back() == '[') key.pop_back();
        isArray = true;
    }

    if (!target->contains(key)) return;

    if (isArray) {
        json& arr = (*target)[key];
        for (auto it = arr.begin(); it != arr.end(); ++it) {
            if (it->is_object() && it->contains(name)) {
                arr.erase(it);
                break;
            }
        }
    } else {
        (*target)[key].erase(name);
    }

    saveConfigToFile(j);
}

nlohmann::json getConfigValue(const std::string& path, const std::string& name) {
    json j = loadConfig();
    json* target = &j;
    std::string key = path;
    bool isArray = false;

    if (!path.empty() && path.back() == '}') {
        key.pop_back();
        if (!key.empty() && key.back() == '{') key.pop_back();
    } else if (!path.empty() && path.back() == ']') {
        key.pop_back();
        if (!key.empty() && key.back() == '[') key.pop_back();
        isArray = true;
    }

    if (!target->contains(key)) return nullptr;

    if (isArray) {
        json& arr = (*target)[key];
        for (auto& el : arr) {
            if (el.is_object() && el.contains(name)) {
                return el[name];
            }
        }
        return nullptr;
    } else {
        if ((*target)[key].contains(name)) {
            return (*target)[key][name];
        }
        return nullptr;
    }
}

std::string getConfigString(const std::string& path, const std::string& name, const std::string& fallback) {
    auto val = getConfigValue(path, name);
    if (val.is_string()) return val.get<std::string>();
    return fallback;
}

bool saveToFile(const std::string& filePath, const nlohmann::json& j) {
    try {
        std::ofstream o(filePath);
        o << j.dump(4);
        return true;
    } catch (...) {
        return false;
    }
}

nlohmann::json loadFromFile(const std::string& filePath) {
    json j;
    try {
        if (fs::exists(filePath)) {
            std::ifstream i(filePath);
            i >> j;
        }
    } catch (...) {
        j = json::object();
    }
    return j;
}

void saveConfigToFilePath(const std::string& filePath, const std::string& path, const std::string& name, const nlohmann::json& value) {
    std::string finalFile = filePath;
    if (filePath == "blush_dir") finalFile = BASE_DIR + "/config.json";

    json j = loadFromFile(finalFile);
    json* target = &j;
    std::string key = path;
    bool isArray = false;

    if (!path.empty() && path.back() == '}') {
        key.pop_back();
        if (!key.empty() && key.back() == '{') key.pop_back();
    } else if (!path.empty() && path.back() == ']') {
        key.pop_back();
        if (!key.empty() && key.back() == '[') key.pop_back();
        isArray = true;
    }

    if (!target->contains(key)) {
        if (isArray) (*target)[key] = json::array();
        else (*target)[key] = json::object();
    }

    if (isArray) {
        json& arr = (*target)[key];
        bool found = false;
        for (auto& el : arr) {
            if (el.is_object() && el.contains(name)) {
                el[name] = value;
                found = true;
                break;
            }
        }
        if (!found) arr.push_back({{name, value}});
    } else {
        (*target)[key][name] = value;
    }

    saveToFile(finalFile, j);
}

nlohmann::json getConfigValueFromFile(const std::string& filePath, const std::string& path, const std::string& name) {
    std::string finalFile = filePath;
    if (filePath == "blush_dir") finalFile = BASE_DIR + "/config.json";

    json j = loadFromFile(finalFile);
    json* target = &j;
    std::string key = path;
    bool isArray = false;

    if (!path.empty() && path.back() == '}') {
        key.pop_back();
        if (!key.empty() && key.back() == '{') key.pop_back();
    } else if (!path.empty() && path.back() == ']') {
        key.pop_back();
        if (!key.empty() && key.back() == '[') key.pop_back();
        isArray = true;
    }

    if (!target->contains(key)) return nullptr;

    if (isArray) {
        json& arr = (*target)[key];
        for (auto& el : arr) {
            if (el.is_object() && el.contains(name)) {
                return el[name];
            }
        }
        return nullptr;
    } else {
        if ((*target)[key].contains(name)) {
            return (*target)[key][name];
        }
        return nullptr;
    }
}
