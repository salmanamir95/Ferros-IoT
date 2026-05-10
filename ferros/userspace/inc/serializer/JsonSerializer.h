#pragma once

#include "serializer/ISerializer.h"
#include <string>

class JsonSerializer final : public ISerializer {
public:
    std::string serialize(const IInsight& insight) override {
        return insight.data().dump();
    }

    std::string getFilePath(const IInsight& insight, uint64_t timestamp) override {
        // output/<analysis_type>/<insight_type>/<timestamp>.json
        std::string analysisType;
        switch(insight.source()) {
            case TelemetryType::SchedulerSwitch: analysisType = "scheduler"; break;
            case TelemetryType::ProcessFork: 
            case TelemetryType::ProcessExit: analysisType = "process"; break;
            default: analysisType = "unknown"; break;
        }

        std::string path = "output/";
        path += analysisType;
        path += "/";
        path += insight.type();
        path += "/";
        path += std::to_string(timestamp) + ".json";
        return path;
    }
};
