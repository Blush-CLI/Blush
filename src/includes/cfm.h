#pragma once
#include <string>
#include <curl/curl.h>

namespace cfm {
    inline size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
        size_t totalSize = size * nmemb;
        std::string* buffer = static_cast<std::string*>(userp);
        buffer->append(static_cast<char*>(contents), totalSize);
        return totalSize;
    }

    inline std::string fetch(const std::string& url) {
        CURL* curl = curl_easy_init();
        std::string response;
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                response = "err_" + std::string(curl_easy_strerror(res));
            }
            curl_easy_cleanup(curl);
        } else {
            response = "err_init_curl";
        }
        return response;
    }

    inline std::string fetch(const std::string& url, const std::string& certPath) {
        CURL* curl = curl_easy_init();
        std::string response;
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            if (!certPath.empty()) {
                curl_easy_setopt(curl, CURLOPT_CAINFO, certPath.c_str());
            }
            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                response = "err_" + std::string(curl_easy_strerror(res));
            }
            curl_easy_cleanup(curl);
        } else {
            response = "err_init_curl";
        }
        return response;
    }
}
