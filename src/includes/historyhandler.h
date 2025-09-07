/*
 File: src/includes/historyhandler.h
 Created: [25.08.2025]
 Description: Defines for some history cpp file
*/

#pragma once
#include <iostream>
#include <vector>
#include <string>

void addHistory(const std::string& content);
std::vector<std::string> getHistory();
void printHistory();
void resetHistory();