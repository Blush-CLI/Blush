#pragma once

#define VERSION "1.0.0"
#define DEBUG true

#ifdef DEBUG
    #define D_PRINT(x, ...) std::print("[DEBUG] " x "\n", ##__VA_ARGS__)
    #define D_PRINTLN(x, ...) std::println("[DEBUG] " x "\n", ##__VA_ARGS__) 
#else
    #define D_PRINT(x, ...)
    #define D_PRINTLN(x, ...)
#endif