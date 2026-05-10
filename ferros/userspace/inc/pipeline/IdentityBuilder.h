#pragma once
#include <nlohmann/json.hpp>
#include "telemetry/ITelemetry.h"

class IdentityBuilder {
public:
    static nlohmann::json build(const ITelemetry& t) {
        nlohmann::json j = {
            {"pid", t.pid()},
            {"tgid", t.tgid()},
            {"comm", t.comm()}
        };
        if (t.ppid() != 0) {
            j["ppid"] = t.ppid();
        } else {
            j["ppid"] = 0; // Explicitly 0
        }
        return j;
    }

    static nlohmann::json buildGlobal() {
        return {
            {"pid", 0},
            {"tgid", 0},
            {"comm", "system"},
            {"scope", "global"}
        };
    }
};
