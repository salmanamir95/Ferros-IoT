#pragma once

#include <memory>
#include "insight/BaseInsight.h"
#include "pipeline/IdentityBuilder.h"
#include "pipeline/TraceBuilder.h"
#include "pipeline/CorrelationBuilder.h"

class InsightFactory {
public:
    static std::unique_ptr<IInsight> createProcessInsight(
        const std::string& type,
        const ITelemetry& telemetry,
        uint64_t fork_ts,
        nlohmann::json metrics
    ) {
        return std::make_unique<BaseInsight>(
            type,
            telemetry.type(),
            IdentityBuilder::build(telemetry),
            TraceBuilder::build(telemetry),
            CorrelationBuilder::build(telemetry.pid(), telemetry.tgid(), fork_ts, telemetry.comm()),
            std::move(metrics)
        );
    }

    static std::unique_ptr<IInsight> createGlobalInsight(
        const std::string& type,
        const ITelemetry& telemetry,
        nlohmann::json metrics
    ) {
        return std::make_unique<BaseInsight>(
            type,
            telemetry.type(),
            IdentityBuilder::buildGlobal(),
            TraceBuilder::build(telemetry),
            CorrelationBuilder::buildGlobal(telemetry.timestamp()),
            std::move(metrics)
        );
    }
};
