#include "EnvLoader.h"
#include "MQTTSubscriber.h"
#include "MessageRouter.h"
#include "ConsoleLogger.h"
#include "FileStorage.h"
#include <iostream>
#include <memory>
#include <filesystem>
#include <thread>
#include <chrono>

int main() {
    std::string envPath = std::filesystem::current_path().string() + "/env/.env.subscriber";
    EnvLoader::load(envPath);

    std::string mqttHost = EnvLoader::get("MQTT_HOST", "127.0.0.1");
    int mqttPort = std::stoi(EnvLoader::get("MQTT_PORT", "1883"));
    std::string mqttTopic = EnvLoader::get("MQTT_TOPIC", "ferros/+/insights");
    std::string mqttClientId = EnvLoader::get("MQTT_CLIENT_ID", "laptop-broker-01");
    int mqttQos = std::stoi(EnvLoader::get("MQTT_QOS", "1"));

    auto router = std::make_shared<MessageRouter>();
    
    // Add handlers
    router->addHandler(std::make_shared<ConsoleLogger>());
    router->addHandler(std::make_shared<FileStorage>("output"));

    MQTTSubscriber subscriber(mqttHost, mqttPort, mqttTopic, mqttClientId, mqttQos, router);

    subscriber.start();

    std::cout << "Broker is running. Press Ctrl+C to exit.\n";

    // Keep main thread alive
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
