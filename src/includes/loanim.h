// src/includes/loanim.h
#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <string>
#include <mutex>

inline std::atomic<bool> _la_running{false};
inline std::thread _la_thread;
inline std::string _la_text = "";
inline std::mutex _la_mutex;

inline void _la_animt(const std::string& text, int durationMs) {
    std::string frames = "|/-\\";
    int frameCount = frames.size();
    int elapsed = 0;
    int step = 100;

    while (elapsed < durationMs) {
        std::cout << "\r" << text << " "
                  << frames[(elapsed / step) % frameCount]
                  << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(step));
        elapsed += step;
    }
    std::cout << "\r" << std::string(text.size() + 4, ' ') << "\r";
}

inline void _la_animgo(const std::string& text) {
    if (_la_running) return; // already running
    _la_running = true;
    {
        std::lock_guard<std::mutex> lock(_la_mutex);
        _la_text = text;
    }

    _la_thread = std::thread([] {
        std::string frames = "|/-\\";
        int i = 0;
        while (_la_running) {
            std::string currentText;
            {
                std::lock_guard<std::mutex> lock(_la_mutex);
                currentText = _la_text;
            }

            std::cout << "\r" << currentText << " "
                      << frames[i % frames.size()]
                      << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            i++;
        }
        std::cout << "\r" << std::string(_la_text.size() + 4, ' ') << "\r";
    });
}

inline void _la_animstop() {
    if (!_la_running) return; // not running
    _la_running = false;
    if (_la_thread.joinable()) {
        _la_thread.join();
    }
}

inline void _la_anim_edit(const std::string& newText) {
    if (!_la_running) return; // do nothing if no anim
    std::lock_guard<std::mutex> lock(_la_mutex);
    _la_text = newText;
}
