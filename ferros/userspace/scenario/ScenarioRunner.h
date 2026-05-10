#ifndef FERROS_SCENARIO_RUNNER_H
#define FERROS_SCENARIO_RUNNER_H

#include "ScenarioRegistry.h"
#include "GlobalConstraints.h"
#include <memory>
#include <string>

namespace ferros {
namespace simulation {

class ScenarioRunner {
private:
    ScenarioRegistry registry_;
    SystemThreshold current_threshold_;

    // Simulates reading current system metrics
    double getCurrentCpuLoad() const;
    double getCurrentMemoryMb() const;
    int getCurrentActiveThreads() const;
    int getCurrentEventsPerSec() const;

    bool isSystemSafe() const;
    void emitOutput(const std::shared_ptr<IScenario>& scenario, const std::string& system_state) const;

public:
    ScenarioRunner();
    void runOnce();
};

} // namespace simulation
} // namespace ferros

#endif // FERROS_SCENARIO_RUNNER_H
