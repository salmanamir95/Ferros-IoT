#pragma once
#include <nlohmann/json.hpp>
#include "telemetry/ITelemetry.h"

class TraceBuilder {
public:
    static nlohmann::json build(const ITelemetry& t) {
        return {
            {"event_type", t.typeString()},
            {"cpu", t.cpu()},
            {"timestamp_ns", t.timestamp()}
        };
    }
};
