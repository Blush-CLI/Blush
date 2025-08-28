#pragma once
#include <string>
#include "../utils/json.hpp"

nlohmann::json loadConfig();
void saveConfigToFile(const nlohmann::json& j);
void saveConfig(const std::string& path, const std::string& name, const nlohmann::json& value);
void removeConfig(const std::string& path, const std::string& name);
nlohmann::json getConfigValue(const std::string& path, const std::string& name);
std::string getConfigString(const std::string& path, const std::string& name, const std::string& fallback="");