#pragma once

#include "IHandler.h"
#include <iostream>

class ConsoleLogger : public IHandler {
public:
    void handle(const nlohmann::json& payload, const std::string& source) override {
        std::cout << "[ConsoleLogger][" << source << "] Received Insight:\n" 
                  << payload.dump(4) << "\n----------------------------------------\n";
    }
};
