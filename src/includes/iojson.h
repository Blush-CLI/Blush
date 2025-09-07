/*
 File: src/includes/iojson.h
 Created: [25.08.2025]
 Description: For Config managing and all with json
*/

#pragma once
#include <string>
#include "../utils/json.hpp"

nlohmann::json loadConfig();
void saveConfigToFile(const nlohmann::json& j);
void saveConfig(const std::string& path, const std::string& name, const nlohmann::json& value);
void removeConfig(const std::string& path, const std::string& name);
nlohmann::json getConfigValue(const std::string& path, const std::string& name);
std::string getConfigString(const std::string& path, const std::string& name, const std::string& fallback="");
bool saveToFile(const std::string& filePath, const nlohmann::json& j);
nlohmann::json loadFromFile(const std::string& filePath);
void saveConfigToFilePath(const std::string& filePath, const std::string& path, const std::string& name, const nlohmann::json& value);
nlohmann::json getConfigValueFromFile(const std::string& filePath, const std::string& path, const std::string& name);
