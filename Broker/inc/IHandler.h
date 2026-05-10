#pragma once

#include <nlohmann/json.hpp>

class IHandler {
public:
    virtual ~IHandler() = default;
    virtual void handle(const nlohmann::json& payload, const std::string& source) = 0;
};
