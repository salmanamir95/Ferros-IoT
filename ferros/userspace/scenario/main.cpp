#include "ScenarioRunner.cpp"
#include "ScenarioRegistry.cpp"
#include <iostream>

int main(int argc, char* argv[]) {
    ferros::simulation::ScenarioRunner runner;

    if (argc > 1) {
        std::string target_scenario = argv[1];
        std::cout << "Starting manual stress: " << target_scenario << "\n";
        // Run infinitely until user hits Ctrl+C
        while (true) {
            // Force true bypasses the stubbed limits to actually cause stress
            runner.runScenario(target_scenario, true);
        }
    } else {
        std::cout << "Running random dataset generation (10 iterations)\n";
        // Run 10 iterations of the scenario generator for dataset generation
        for (int i = 0; i < 10; ++i) {
            runner.runOnce();
        }
    }

    return 0;
}
