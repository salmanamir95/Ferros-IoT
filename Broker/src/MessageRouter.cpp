#include "MessageRouter.h"
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void MessageRouter::addHandler(std::shared_ptr<IHandler> handler) {
    handlers_.push_back(handler);
}

void MessageRouter::route(const std::string& rawMessage) {
    try {
        // Parse ONCE
        json wrapper = json::parse(rawMessage);
        
        std::string source = wrapper.value("source", "unknown");
        
        if (wrapper.contains("payload")) {
            const json& payload = wrapper["payload"];
            
            for (auto& handler : handlers_) {
                handler->handle(payload, source);
            }
        } else {
            std::cerr << "[MessageRouter] Missing 'payload' in wrapper.\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "[MessageRouter] JSON Parse Error: " << e.what() << "\nRaw: " << rawMessage << "\n";
    }
}
