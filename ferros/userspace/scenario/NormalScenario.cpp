#ifndef FERROS_NORMAL_SCENARIO_CPP
#define FERROS_NORMAL_SCENARIO_CPP

#include "../scenario/IScenario.h"
#include "../scenario/GlobalConstraints.h"
#include <thread>
#include <chrono>

namespace ferros {
namespace simulation {

class NormalScenario : public IScenario {
private:
    int duration_sec_;

public:
    NormalScenario() {
        duration_sec_ = std::min(5, MAX_DURATION_SEC); // Default normal duration
    }

    void execute() override {
        // Normal behavior is just waiting, not stressing the system
        std::this_thread::sleep_for(std::chrono::seconds(duration_sec_));
    }

    std::string getName() const override {
        return "NormalScenario";
    }

    std::string getType() const override {
        return "normal";
    }

    int getDurationSec() const override {
        return duration_sec_;
    }

    int getCpuThreads() const override {
        return 0;
    }
};

} // namespace simulation
} // namespace ferros

#endif // FERROS_NORMAL_SCENARIO_CPP
