#pragma once

#include <string>
#include <memory>
#include <mqtt/async_client.h>
#include "MessageRouter.h"

class MQTTSubscriber : public virtual mqtt::callback {
public:
    MQTTSubscriber(const std::string& host, int port, const std::string& topic, const std::string& clientId, int qos, std::shared_ptr<MessageRouter> router);
    ~MQTTSubscriber();

    void start();
    void stop();

private:
    void connection_lost(const std::string& cause) override;
    void message_arrived(mqtt::const_message_ptr msg) override;
    void delivery_complete(mqtt::delivery_token_ptr token) override;
    void connected(const std::string& cause) override;

    std::string brokerUrl_;
    std::string topic_;
    std::string clientId_;
    int qos_;

    std::unique_ptr<mqtt::async_client> client_;
    std::shared_ptr<MessageRouter> router_;
};
