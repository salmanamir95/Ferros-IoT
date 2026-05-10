#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <utility>
#include <string>

#include "telemetry/ITelemetry.h"
#include "telemetry/TelemetryType.h"
#include "analyzer/IAnalyzer.h"
#include "insight/IInsight.h"
#include "intelligence/IIntelligence.h"
#include "serializer/ISerializer.h"
#include "publisher/IPublisher.h"

class Pipeline final {
public:
    Pipeline() = default;
    ~Pipeline() = default;

    Pipeline(const Pipeline&) = delete;
    Pipeline& operator=(const Pipeline&) = delete;

    void registerAnalyzer(TelemetryType type, std::unique_ptr<IAnalyzer> analyzer) {
        analyzers[type].push_back(std::move(analyzer));
    }

    void setIntelligence(std::unique_ptr<IIntelligence> intel) {
        intelligence = std::move(intel);
    }

    void setSerializer(std::unique_ptr<ISerializer> ser) {
        serializer = std::move(ser);
    }

    void addPublisher(std::unique_ptr<IPublisher> pub) {
        publishers.push_back(std::move(pub));
    }

    void process(std::unique_ptr<ITelemetry> telemetry) {
        if (!telemetry || !serializer) return;

        TelemetryType type = telemetry->type();
        auto it = analyzers.find(type);
        if (it == analyzers.end() || it->second.empty()) {
            return;
        }

        for (auto& analyzer : it->second) {
            std::vector<std::unique_ptr<IInsight>> rawInsights = analyzer->analyze(*telemetry);
            
            for (auto& rawInsight : rawInsights) {
                if (!rawInsight) continue;

                std::unique_ptr<IInsight> finalInsight = std::move(rawInsight);
                
                if (intelligence) {
                    finalInsight = intelligence->interpret(std::move(finalInsight));
                }

                if (!finalInsight) continue;

                std::string serializedData = serializer->serialize(*finalInsight);
                if (serializedData.empty() || serializedData == "{}") continue;

                std::string filepath = serializer->getFilePath(*finalInsight, telemetry->timestamp());

                for (auto& publisher : publishers) {
                    publisher->publish(serializedData, filepath);
                }
            }
        }
    }

private:
    std::unordered_map<TelemetryType, std::vector<std::unique_ptr<IAnalyzer>>> analyzers;
    std::unique_ptr<IIntelligence> intelligence;
    std::unique_ptr<ISerializer> serializer;
    std::vector<std::unique_ptr<IPublisher>> publishers;
};
