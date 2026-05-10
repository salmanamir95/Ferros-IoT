#include "MQTTSubscriber.h"
#include <iostream>

MQTTSubscriber::MQTTSubscriber(const std::string& host, int port, const std::string& topic, const std::string& clientId, int qos, std::shared_ptr<MessageRouter> router)
    : brokerUrl_("tcp://" + host + ":" + std::to_string(port)),
      topic_(topic),
      clientId_(clientId),
      qos_(qos),
      router_(router)
{
    client_ = std::make_unique<mqtt::async_client>(brokerUrl_, clientId_);
    client_->set_callback(*this);
}

MQTTSubscriber::~MQTTSubscriber() {
    stop();
}

void MQTTSubscriber::start() {
    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(20);
    connOpts.set_clean_session(true);
    connOpts.set_automatic_reconnect(1, 10);

    try {
        std::cout << "[MQTTSubscriber] Connecting to " << brokerUrl_ << "..." << std::endl;
        client_->connect(connOpts)->wait();
    } catch (const mqtt::exception& exc) {
        std::cerr << "[MQTTSubscriber] Connection error: " << exc.what() << std::endl;
    }
}

void MQTTSubscriber::stop() {
    if (client_ && client_->is_connected()) {
        client_->disconnect()->wait();
    }
}

void MQTTSubscriber::connection_lost(const std::string& cause) {
    std::cout << "[MQTTSubscriber] Connection lost: " << cause << std::endl;
}

void MQTTSubscriber::connected(const std::string& cause) {
    std::cout << "[MQTTSubscriber] Connected! Subscribing to " << topic_ << std::endl;
    client_->subscribe(topic_, qos_)->wait();
}

void MQTTSubscriber::message_arrived(mqtt::const_message_ptr msg) {
    if (router_ && msg) {
        router_->route(msg->to_string());
    }
}

void MQTTSubscriber::delivery_complete(mqtt::delivery_token_ptr token) {
    // Not needed for subscriber
}
