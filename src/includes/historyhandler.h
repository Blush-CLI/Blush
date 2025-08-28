#pragma once
#include <iostream>
#include <vector>
#include <string>

void addHistory(const std::string& content);
std::vector<std::string> getHistory();
void printHistory();
void resetHistory();