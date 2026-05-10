#pragma once

#include <string>
#include <cstdint>
#include "insight/IInsight.h"

class ISerializer {
public:
    virtual ~ISerializer() = default;

    virtual std::string serialize(const IInsight& insight) = 0;
    
    // Generates the hierarchical file path for the given insight and timestamp.
    virtual std::string getFilePath(const IInsight& insight, uint64_t timestamp) = 0;
};
