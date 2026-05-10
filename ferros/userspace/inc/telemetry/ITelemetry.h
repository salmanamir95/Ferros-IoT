#pragma once

#include <cstdint>
#include <string>
#include "telemetry/TelemetryType.h"

class ITelemetry {
public:
    virtual ~ITelemetry() = default;

    virtual TelemetryType type() const noexcept = 0;
    virtual std::string typeString() const noexcept = 0;
    
    virtual uint64_t timestamp() const noexcept = 0;

    virtual uint32_t pid() const noexcept = 0;
    virtual uint32_t tgid() const noexcept = 0;
    virtual uint32_t ppid() const noexcept = 0;
    virtual uint32_t cpu() const noexcept = 0;
    virtual std::string comm() const noexcept = 0;
};
