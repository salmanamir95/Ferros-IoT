#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include "telemetry/TelemetryType.h"

class IInsight {
public:
    virtual ~IInsight() = default;

    virtual std::string type() const = 0;
    virtual TelemetryType source() const = 0;

    virtual nlohmann::json data() const = 0;
};
