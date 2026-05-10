#ifndef FERROS_CPU_BURST_STRESS_CPP
#define FERROS_CPU_BURST_STRESS_CPP

#include "../scenario/IScenario.h"
#include "../scenario/GlobalConstraints.h"
#include <thread>
#include <vector>
#include <chrono>
#include <cmath>

namespace ferros {
namespace simulation {

class CpuBurstStress : public IScenario {
private:
    int duration_sec_;
    int threads_;

    void burnCPU() {
        auto start = std::chrono::steady_clock::now();
        volatile double result = 0.0;
        while (std::chrono::steady_clock::now() - start < std::chrono::seconds(duration_sec_)) {
            for (int i = 0; i < 1000; ++i) {
                result += std::sin(i) * std::cos(i);
            }
        }
    }

public:
    CpuBurstStress() {
        duration_sec_ = std::min(5, MAX_DURATION_SEC);
        threads_ = std::min(2, MAX_THREADS);
    }

    void execute() override {
        std::vector<std::thread> workers;
        for (int i = 0; i < threads_; ++i) {
            workers.emplace_back(&CpuBurstStress::burnCPU, this);
        }
        for (auto& t : workers) {
            if (t.joinable()) {
                t.join();
            }
        }
    }

    std::string getName() const override {
        return "CpuBurstStress";
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

#endif // FERROS_CPU_BURST_STRESS_CPP
