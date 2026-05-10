#ifndef FERROS_PROCESS_STRESS_CPP
#define FERROS_PROCESS_STRESS_CPP

#include "../scenario/IScenario.h"
#include "../scenario/GlobalConstraints.h"
#include <thread>
#include <chrono>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>

namespace ferros {
namespace simulation {

class ProcessStress : public IScenario {
private:
    int duration_sec_;

public:
    ProcessStress() {
        duration_sec_ = std::min(5, MAX_DURATION_SEC);
    }

    void execute() override {
        auto start = std::chrono::steady_clock::now();
        int safe_child_limit = std::min(5, MAX_CHILD_PROCESSES);

        while (std::chrono::steady_clock::now() - start < std::chrono::seconds(duration_sec_)) {
            for (int i = 0; i < safe_child_limit; ++i) {
                pid_t pid = fork();
                if (pid == 0) {
                    // Child process: do nothing and exit immediately
                    _exit(0);
                } else if (pid > 0) {
                    // Parent waits for the specific child
                    waitpid(pid, nullptr, 0);
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

    std::string getName() const override {
        return "ProcessStress";
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

#endif // FERROS_PROCESS_STRESS_CPP
