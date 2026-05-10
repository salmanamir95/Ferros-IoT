#include "ScenarioRunner.cpp"
#include "ScenarioRegistry.cpp"

int main() {
    ferros::simulation::ScenarioRunner runner;

    // Run 10 iterations of the scenario generator for dataset generation
    for (int i = 0; i < 10; ++i) {
        runner.runOnce();
    }

    return 0;
}
