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
        std::string filename = dir_ + "/" + source + "_insights.jsonl";
        std::ofstream out(filename, std::ios::app);
        if (out.is_open()) {
            out << payload.dump() << "\n";
        } else {
            std::cerr << "[FileStorage] Failed to open " << filename << " for writing.\n";
        }
    }
private:
    std::string dir_;
};
