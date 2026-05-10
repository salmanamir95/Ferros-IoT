#pragma once

#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <cstdlib>

class EnvLoader {
public:
    static void load(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) return;

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;

            auto pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                
                // Trim whitespace (basic implementation)
                key.erase(0, key.find_first_not_of(" \t"));
                key.erase(key.find_last_not_of(" \t") + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t") + 1);

                setenv(key.c_str(), value.c_str(), 1);
            }
        }
    }

    static std::string get(const std::string& key, const std::string& defaultValue = "") {
        const char* val = std::getenv(key.c_str());
        return val ? std::string(val) : defaultValue;
    }
};
