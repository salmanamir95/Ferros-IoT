#ifndef FERROS_MEMORY_STRESS_CPP
#define FERROS_MEMORY_STRESS_CPP

#include "../scenario/IScenario.h"
#include "../scenario/GlobalConstraints.h"
#include <thread>
#include <chrono>
#include <vector>

namespace ferros {
namespace simulation {

class MemoryStress : public IScenario {
private:
    int duration_sec_;

public:
    MemoryStress() {
        duration_sec_ = std::min(5, MAX_DURATION_SEC);
    }

    void execute() override {
        // Strict boundary checking for memory
        size_t memory_bytes = static_cast<size_t>(std::min(40, MAX_MEMORY_MB)) * 1024 * 1024;
        std::vector<char> buffer;
        
        try {
            buffer.resize(memory_bytes);
            auto start = std::chrono::steady_clock::now();
            
            while (std::chrono::steady_clock::now() - start < std::chrono::seconds(duration_sec_)) {
                // Touch memory to bypass lazy allocation
                for (size_t i = 0; i < memory_bytes; i += 4096) {
                    buffer[i] = static_cast<char>(i % 256);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        } catch (...) {
            // Ensure no crashing if memory fails
        }
    }

    std::string getName() const override {
        return "MemoryStress";
    }

    std::string getType() const override {
        return "attack";
    }

    int getDurationSec() const override {
        return duration_sec_;
    }

    int getCpuThreads() const override {
        return 1;
    }
};

} // namespace simulation
} // namespace ferros

#endif // FERROS_MEMORY_STRESS_CPP
