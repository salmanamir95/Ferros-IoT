#ifndef FERROS_SCENARIO_REGISTRY_H
#define FERROS_SCENARIO_REGISTRY_H

#include "IScenario.h"
#include <memory>
#include <vector>

namespace ferros {
namespace simulation {

class ScenarioRegistry {
private:
    std::vector<std::shared_ptr<IScenario>> scenarios_;

public:
    ScenarioRegistry();

    // Register all available scenarios
    void initialize();

    // Randomly select a scenario
    std::shared_ptr<IScenario> getRandomScenario() const;

    // Get a specific scenario by name
    std::shared_ptr<IScenario> getScenarioByName(const std::string& name) const;
};

} // namespace simulation
} // namespace ferros

#endif // FERROS_SCENARIO_REGISTRY_H
