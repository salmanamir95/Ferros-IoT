#ifndef FERROS_IO_BURST_STRESS_CPP
#define FERROS_IO_BURST_STRESS_CPP

#include "../scenario/IScenario.h"
#include "../scenario/GlobalConstraints.h"
#include <thread>
#include <chrono>
#include <fstream>
#include <cstdio>
#include <vector>

namespace ferros {
namespace simulation {

class IoBurstStress : public IScenario {
private:
    int duration_sec_;
    const std::string tmp_file_ = "/tmp/ferros_io_stress.tmp";

public:
    IoBurstStress() {
        duration_sec_ = std::min(5, MAX_DURATION_SEC);
    }

    void execute() override {
        auto start = std::chrono::steady_clock::now();
        std::vector<char> buffer(1024 * 1024, 'A'); // 1MB buffer

        while (std::chrono::steady_clock::now() - start < std::chrono::seconds(duration_sec_)) {
            // Write to temp file
            std::ofstream out(tmp_file_, std::ios::binary);
            if (out) {
                out.write(buffer.data(), buffer.size());
                out.close();
            }

            // Read from temp file
            std::ifstream in(tmp_file_, std::ios::binary);
            if (in) {
                std::vector<char> read_buf(1024 * 1024);
                in.read(read_buf.data(), read_buf.size());
                in.close();
            }

            // Remove to prevent disk filling
            std::remove(tmp_file_.c_str());

            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }

    std::string getName() const override {
        return "IoBurstStress";
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

#endif // FERROS_IO_BURST_STRESS_CPP
