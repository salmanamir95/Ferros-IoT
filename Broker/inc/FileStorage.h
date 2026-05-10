#pragma once

#include "IHandler.h"
#include <fstream>
#include <iostream>
#include <filesystem>

class FileStorage : public IHandler {
public:
    FileStorage(const std::string& directory) : dir_(directory) {
        if (!std::filesystem::exists(dir_)) {
            std::filesystem::create_directories(dir_);
        }
    }

    void handle(const nlohmann::json& payload, const std::string& source) override {
        (void)source; // Unused in the new structure
        
        std::string analysisType = "unknown";
        std::string insightType = "unknown";
        std::string timestamp = "0";

        try {
            if (payload.contains("trace") && payload["trace"].contains("event_type")) {
                std::string eventType = payload["trace"]["event_type"];
                if (eventType == "SchedulerSwitch") {
                    analysisType = "scheduler";
                } else if (eventType == "ProcessFork" || eventType == "ProcessExit") {
                    analysisType = "process";
                }
            }

            if (payload.contains("type")) {
                insightType = payload["type"];
            }

            if (payload.contains("trace") && payload["trace"].contains("timestamp_ns")) {
                timestamp = std::to_string(payload["trace"]["timestamp_ns"].get<uint64_t>());
            }
        } catch (const std::exception& e) {
            std::cerr << "[FileStorage] Error parsing payload for path: " << e.what() << "\n";
        }

        std::filesystem::path dirPath = std::filesystem::path(dir_) / analysisType / insightType;
        
        if (!std::filesystem::exists(dirPath)) {
            std::error_code ec;
            std::filesystem::create_directories(dirPath, ec);
            if (ec) {
                std::cerr << "[FileStorage] Failed to create directory: " << dirPath << "\n";
                return;
            }
        }

        std::string filename = (dirPath / (timestamp + ".json")).string();
        
        // Write the payload as a single JSON object, mimicking ferros LocalStorage behavior
        std::ofstream out(filename, std::ios::out | std::ios::trunc);
        if (out.is_open()) {
            out << payload.dump() << "\n";
            out.close();
        } else {
            std::cerr << "[FileStorage] Failed to open " << filename << " for writing.\n";
        }
    }
private:
    std::string dir_;
};
