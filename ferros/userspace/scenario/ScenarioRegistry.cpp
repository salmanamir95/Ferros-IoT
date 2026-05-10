#ifndef FERROS_SCENARIO_REGISTRY_CPP
#define FERROS_SCENARIO_REGISTRY_CPP

#include "ScenarioRegistry.h"
#include "../attack-scripts/CpuBurstStress.cpp"
#include "../attack-scripts/MemoryStress.cpp"
#include "../attack-scripts/ProcessStress.cpp"
#include "../attack-scripts/SchedulerStress.cpp"
#include "../attack-scripts/IoBurstStress.cpp"
#include "NormalScenario.cpp"

#include <random>

namespace ferros {
namespace simulation {

ScenarioRegistry::ScenarioRegistry() {
    initialize();
}

void ScenarioRegistry::initialize() {
    scenarios_.push_back(std::make_shared<CpuBurstStress>());
    scenarios_.push_back(std::make_shared<MemoryStress>());
    scenarios_.push_back(std::make_shared<ProcessStress>());
    scenarios_.push_back(std::make_shared<SchedulerStress>());
    scenarios_.push_back(std::make_shared<IoBurstStress>());
    scenarios_.push_back(std::make_shared<NormalScenario>());
}

std::shared_ptr<IScenario> ScenarioRegistry::getRandomScenario() const {
    if (scenarios_.empty()) return nullptr;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, scenarios_.size() - 1);

    return scenarios_[dist(gen)];
}

} // namespace simulation
} // namespace ferros

#endif // FERROS_SCENARIO_REGISTRY_CPP
