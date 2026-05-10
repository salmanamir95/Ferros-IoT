#include <iostream>
#include <memory>

#include "pipeline/Pipeline.h"
#include "pipeline/EBPFReader.h"
#include "analyzer/CpuAnalyzer.h"
#include "analyzer/SchedulerAnalyzer.h"
#include "analyzer/ProcessAnalyzer.h"
#include "intelligence/BayesianBinaryClassifier.h"
#include "serializer/JsonSerializer.h"
#include "publisher/LocalStorage.h"
#include "publisher/MQTTPublisher.h"
#include "EnvLoader.h"
#include <filesystem>

int main()
{
    // Load environment configuration
    std::string envPath = std::filesystem::current_path().string() + "/env/.env.publisher";
    EnvLoader::load(envPath);

    Pipeline pipeline;

    // Analyzers
    pipeline.registerAnalyzer(TelemetryType::SchedulerSwitch, std::make_unique<SchedulerAnalyzer>());
    pipeline.registerAnalyzer(TelemetryType::SchedulerSwitch, std::make_unique<CpuAnalyzer>());
    pipeline.registerAnalyzer(TelemetryType::ProcessFork, std::make_unique<ProcessAnalyzer>());
    pipeline.registerAnalyzer(TelemetryType::ProcessExit, std::make_unique<ProcessAnalyzer>());

    // Intelligence
    pipeline.setIntelligence(std::make_unique<BayesianBinaryClassifier>());

    // Serializer & Publisher
    pipeline.setSerializer(std::make_unique<JsonSerializer>());
    pipeline.addPublisher(std::make_unique<LocalStorage>());
    
    std::string mqttHost = EnvLoader::get("MQTT_HOST", "127.0.0.1");
    int mqttPort = std::stoi(EnvLoader::get("MQTT_PORT", "1883"));
    std::string mqttTopic = EnvLoader::get("MQTT_TOPIC", "ferros/pi/insights");
    std::string mqttClientId = EnvLoader::get("MQTT_CLIENT_ID", "ferros-pi");
    int mqttQos = std::stoi(EnvLoader::get("MQTT_QOS", "1"));
    
    pipeline.addPublisher(std::make_unique<MQTTPublisher>(mqttHost, mqttPort, mqttTopic, mqttClientId, mqttQos));

    std::cout << "Starting Ferros Real eBPF Pipeline..." << std::endl;

    EBPFReader reader(pipeline);
    if (!reader.start()) {
        std::cerr << "Failed to start eBPF reader\n";
        return 1;
    }

    // reader.start() blocks as it polls the ringbuffer.
    // If it ever returns, we exit.
    
    return 0;
}