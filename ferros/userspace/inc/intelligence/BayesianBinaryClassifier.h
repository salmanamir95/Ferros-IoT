#pragma once

#include "intelligence/IIntelligence.h"

class BayesianBinaryClassifier final : public IIntelligence {
public:
    std::unique_ptr<IInsight> interpret(std::unique_ptr<IInsight> raw) override {
        // In a real implementation, this would assign a probability score
        // and classify NORMAL / ANOMALY. For now, it acts as a pass-through
        // that could potentially enrich the underlying JSON.
        return raw;
    }
};
