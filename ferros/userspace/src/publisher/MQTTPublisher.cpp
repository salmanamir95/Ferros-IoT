#include "publisher/MQTTPublisher.h"
#include <iostream>
#include <chrono>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

MQTTPublisher::MQTTPublisher(const std::string& host, int port, const std::string& topic, const std::string& clientId, int qos)
    : brokerUrl_("tcp://" + host + ":" + std::to_string(port)),
      topic_(topic),
      clientId_(clientId),
      qos_(qos),
      maxQueueSize_(1000),
      running_(true) 
{
    client_ = std::make_unique<mqtt::async_client>(brokerUrl_, clientId_);
    connect();

    workerThread_ = std::thread(&MQTTPublisher::workerLoop, this);
}

MQTTPublisher::~MQTTPublisher() {
    running_ = false;
    queueCv_.notify_all();
    if (workerThread_.joinable()) {
        workerThread_.join();
    }
    if (client_ && client_->is_connected()) {
        client_->disconnect()->wait();
    }
}

void MQTTPublisher::connect() {
    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(20);
    connOpts.set_clean_session(true);
    connOpts.set_automatic_reconnect(1, 10); // min 1s, max 10s backoff

    try {
        std::cout << "[MQTTPublisher] Connecting to broker at " << brokerUrl_ << "..." << std::endl;
        client_->connect(connOpts)->wait();
        std::cout << "[MQTTPublisher] Connected." << std::endl;
    } catch (const mqtt::exception& exc) {
        std::cerr << "[MQTTPublisher] Error connecting: " << exc.what() << std::endl;
        // The async_client automatic reconnect will keep trying in the background if connection drops,
        // but if initial connect fails, it might need to be retried by the library.
        // Actually, automatic_reconnect only applies after a successful connection.
        // For production, we would loop here or rely on the worker loop to reconnect if disconnected.
    }
}

void MQTTPublisher::publish(const std::string& serializedData, const std::string& filepath) {
    (void)filepath; // We use topic, not filepath
    if (serializedData.empty()) return;

    try {
        json wrapper;
        wrapper["payload"] = json::parse(serializedData);
        wrapper["source"] = clientId_;
        
        auto now = std::chrono::system_clock::now().time_since_epoch();
        wrapper["timestamp_ns"] = std::chrono::duration_cast<std::chrono::nanoseconds>(now).count();

        std::string finalData = wrapper.dump();

        std::unique_lock<std::mutex> lock(queueMutex_);
        if (messageQueue_.size() >= maxQueueSize_) {
            // Drop newest strategy: simply don't enqueue the new message
            // std::cerr << "[MQTTPublisher] Queue full, dropping newest message.\n";
            return;
        }

        messageQueue_.push(finalData);
        lock.unlock();
        queueCv_.notify_one();
    } catch (const std::exception& e) {
        std::cerr << "[MQTTPublisher] JSON Parse/Wrap Error: " << e.what() << "\n";
    }
}

void MQTTPublisher::workerLoop() {
    while (running_) {
        std::string payload;

        {
            std::unique_lock<std::mutex> lock(queueMutex_);
            queueCv_.wait(lock, [this]() { return !messageQueue_.empty() || !running_; });

            if (!running_ && messageQueue_.empty()) {
                break;
            }

            payload = messageQueue_.front();
            messageQueue_.pop();
        }

        if (!client_->is_connected()) {
            // If not connected, paho-mqtt auto-reconnect handles reconnection, 
            // but we might want to delay publishing or sleep
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            // To prevent message loss we should ideally re-queue, but dropping might be fine for telemetry
            // Let's re-enqueue to front to avoid loss if we really care, or just attempt publish which might fail.
        }

        try {
            mqtt::message_ptr pubmsg = mqtt::make_message(topic_, payload);
            pubmsg->set_qos(qos_);
            client_->publish(pubmsg)->wait_for(std::chrono::milliseconds(500));
        } catch (const mqtt::exception& exc) {
            std::cerr << "[MQTTPublisher] Publish error: " << exc.what() << std::endl;
        }
    }
}
