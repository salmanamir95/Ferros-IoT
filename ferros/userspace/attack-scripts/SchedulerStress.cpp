#ifndef FERROS_SCHEDULER_STRESS_CPP
#define FERROS_SCHEDULER_STRESS_CPP

#include "../scenario/IScenario.h"
#include "../scenario/GlobalConstraints.h"
#include <thread>
#include <chrono>
#include <vector>
#include <sched.h>

namespace ferros {
namespace simulation {

class SchedulerStress : public IScenario {
private:
    int duration_sec_;
    int threads_;

    void yieldLoop() {
        auto start = std::chrono::steady_clock::now();
        while (std::chrono::steady_clock::now() - start < std::chrono::seconds(duration_sec_)) {
            // Force a context switch
            sched_yield();
        }
    }

public:
    SchedulerStress() {
        duration_sec_ = std::min(5, MAX_DURATION_SEC);
        threads_ = std::min(4, MAX_THREADS);
    }

    void execute() override {
        std::vector<std::thread> workers;
        for (int i = 0; i < threads_; ++i) {
            workers.emplace_back(&SchedulerStress::yieldLoop, this);
        }
        for (auto& t : workers) {
            if (t.joinable()) {
                t.join();
            }
        }
    }

    std::string getName() const override {
        return "SchedulerStress";
    }

    std::string getType() const override {
        return "attack";
    }

    int getDurationSec() const override {
        return duration_sec_;
    }

    int getCpuThreads() const override {
        return threads_;
    }
};

} // namespace simulation
} // namespace ferros

#endif // FERROS_SCHEDULER_STRESS_CPP
