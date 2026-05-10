#ifndef FERROS_SCENARIO_RUNNER_CPP
#define FERROS_SCENARIO_RUNNER_CPP

#include "ScenarioRunner.h"
#include "NormalScenario.cpp" // Included for fallback
#include <iostream>
#include <chrono>

namespace ferros {
namespace simulation {

ScenarioRunner::ScenarioRunner() {}

double ScenarioRunner::getCurrentCpuLoad() const {
    // In a real implementation, this would read from /proc/stat or eBPF.
    // Simulating safe load for the purposes of the generator logic.
    return 0.50; 
}

double ScenarioRunner::getCurrentMemoryMb() const {
    return 100.0;
}

int ScenarioRunner::getCurrentActiveThreads() const {
    return 2;
}

int ScenarioRunner::getCurrentEventsPerSec() const {
    return 500;
}

bool ScenarioRunner::isSystemSafe() const {
    if (getCurrentCpuLoad() > current_threshold_.max_cpu_load) return false;
    if (getCurrentMemoryMb() > current_threshold_.max_memory_mb) return false;
    if (getCurrentActiveThreads() > current_threshold_.max_active_threads) return false;
    if (getCurrentEventsPerSec() > current_threshold_.max_events_per_sec) return false;
    return true;
}

void ScenarioRunner::emitOutput(const std::shared_ptr<IScenario>& scenario, const std::string& system_state) const {
    auto now = std::chrono::system_clock::now();
    auto timestamp_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();

    // Output strictly matching the requested JSON format
    std::cout << "{\n"
              << "  \"scenario\": \"" << scenario->getName() << "\",\n"
              << "  \"type\": \"" << scenario->getType() << "\",\n"
              << "  \"system_state\": \"" << system_state << "\",\n"
              << "  \"cpu_threads\": " << scenario->getCpuThreads() << ",\n"
              << "  \"duration_sec\": " << scenario->getDurationSec() << ",\n"
              << "  \"timestamp_ns\": " << timestamp_ns << "\n"
              << "}\n";
}

void ScenarioRunner::runOnce() {
    auto scenario = registry_.getRandomScenario();
    std::string system_state = "safe";

    if (!isSystemSafe()) {
        system_state = "throttled";
        // Fallback to normal scenario
        scenario = std::make_shared<NormalScenario>();
    }

    // Execute the bound scenario
    scenario->execute();

    // Emit dataset label
    emitOutput(scenario, system_state);
}

void ScenarioRunner::runScenario(const std::string& name, bool force) {
    auto scenario = registry_.getScenarioByName(name);
    if (!scenario) {
        std::cerr << "Scenario '" << name << "' not found.\n";
        return;
    }

    std::string system_state = "safe";

    // If forcing, bypass safety limits
    if (!force && !isSystemSafe()) {
        system_state = "throttled";
        scenario = std::make_shared<NormalScenario>();
    }

    scenario->execute();
    emitOutput(scenario, system_state);
}

} // namespace simulation
} // namespace ferros

#endif // FERROS_SCENARIO_RUNNER_CPP
