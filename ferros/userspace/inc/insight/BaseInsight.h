#pragma once

#include "insight/IInsight.h"
#include <utility>

class BaseInsight : public IInsight {
public:
    BaseInsight(
        std::string type,
        TelemetryType source,
        nlohmann::json identity,
        nlohmann::json trace,
        std::string correlation_id,
        nlohmann::json metrics
    )
    : _type(std::move(type)),
      _source(source),
      _identity(std::move(identity)),
      _trace(std::move(trace)),
      _correlation_id(std::move(correlation_id)),
      _metrics(std::move(metrics)) {}

    std::string type() const override { return _type; }
    TelemetryType source() const override { return _source; }

    nlohmann::json data() const override {
        return {
            {"type", _type},
            {"source", _source},
            {"identity", _identity},
            {"trace", _trace},
            {"correlation_id", _correlation_id},
            {"metrics", _metrics}
        };
    }

private:
    std::string _type;
    TelemetryType _source;
    nlohmann::json _identity;
    nlohmann::json _trace;
    std::string _correlation_id;
    nlohmann::json _metrics;
};
