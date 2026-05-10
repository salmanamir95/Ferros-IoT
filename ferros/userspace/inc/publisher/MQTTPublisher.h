#pragma once

#include "publisher/IPublisher.h"
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <memory>
#include <mqtt/async_client.h>

class MQTTPublisher final : public IPublisher {
public:
    MQTTPublisher(const std::string& host, int port, const std::string& topic, const std::string& clientId, int qos);
    ~MQTTPublisher() override;

    void publish(const std::string& serializedData, const std::string& filepath) override;

private:
    void workerLoop();
    void connect();

    std::string brokerUrl_;
    std::string topic_;
    std::string clientId_;
    int qos_;

    std::unique_ptr<mqtt::async_client> client_;

    std::queue<std::string> messageQueue_;
    size_t maxQueueSize_;
    std::mutex queueMutex_;
    std::condition_variable queueCv_;
    
    std::thread workerThread_;
    std::atomic<bool> running_;
};
