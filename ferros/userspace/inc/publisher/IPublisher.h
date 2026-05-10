#pragma once

#include <string>

class IPublisher {
public:
    virtual ~IPublisher() = default;

    virtual void publish(const std::string& serializedData, const std::string& filepath) = 0;
};