#ifndef FERROS_GLOBAL_CONSTRAINTS_H
#define FERROS_GLOBAL_CONSTRAINTS_H

namespace ferros {
namespace simulation {

constexpr int MAX_THREADS = 4;
constexpr int MAX_DURATION_SEC = 10;
constexpr int MAX_MEMORY_MB = 50;
constexpr int MAX_CHILD_PROCESSES = 10;

struct SystemThreshold {
    double max_cpu_load = 0.75;
    double max_memory_mb = 200.0;
    int max_active_threads = 4;
    int max_events_per_sec = 1000;
};

} // namespace simulation
} // namespace ferros

#endif // FERROS_GLOBAL_CONSTRAINTS_H
