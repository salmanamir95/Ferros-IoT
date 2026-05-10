#pragma once

#include <memory>
#include "insight/IInsight.h"

class IIntelligence {
public:
    virtual ~IIntelligence() = default;

    // Takes ownership of raw insight, returns filtered/enriched insight,
    // or nullptr if the insight is suppressed (filtered out).
    virtual std::unique_ptr<IInsight> interpret(std::unique_ptr<IInsight> raw) = 0;
};
