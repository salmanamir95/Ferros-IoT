#pragma once

#include <string>
#include <vector>
#include <memory>
#include "IHandler.h"

class MessageRouter {
public:
    void addHandler(std::shared_ptr<IHandler> handler);
    void route(const std::string& rawMessage);

private:
    std::vector<std::shared_ptr<IHandler>> handlers_;
};
