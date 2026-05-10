#pragma once

#include <memory>
#include <vector>
#include "telemetry/ITelemetry.h"
#include "insight/IInsight.h"

class IAnalyzer {
public:
    virtual ~IAnalyzer() = default;

    virtual bool supports(TelemetryType type) const = 0;

    virtual std::vector<std::unique_ptr<IInsight>> analyze(const ITelemetry& telemetry) = 0;
};
