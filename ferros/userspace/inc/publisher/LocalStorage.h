#pragma once

#include "publisher/IPublisher.h"
#include <fstream>
#include <filesystem>
#include <iostream>

class LocalStorage final : public IPublisher {
public:
    void publish(const std::string& serializedData, const std::string& filepath) override {
        std::filesystem::path pathObj(filepath);
        std::filesystem::path dir = pathObj.parent_path();
        
        if (!dir.empty() && !std::filesystem::exists(dir)) {
            std::error_code ec;
            std::filesystem::create_directories(dir, ec);
            if (ec) {
                std::cerr << "Failed to create directory: " << dir << " Error: " << ec.message() << "\n";
                return;
            }
        }

        std::string tempFilepath = filepath + ".tmp";
        std::ofstream out(tempFilepath, std::ios::out | std::ios::trunc);
        if (out.is_open()) {
            out << serializedData;
            out.close();
            
            std::error_code ec;
            std::filesystem::rename(tempFilepath, filepath, ec);
            if (ec) {
                std::cerr << "Failed to rename temp file to: " << filepath << "\n";
            }
        } else {
            std::cerr << "Failed to open temporary file for writing: " << tempFilepath << "\n";
        }
    }
};
