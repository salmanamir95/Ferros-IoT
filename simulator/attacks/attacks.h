#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdint>
#include <unistd.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


bool isCpuAttack(const std::string& n) {
    return n == "cpu_heavy" ||
           n == "sustained_cpu_compute_load" ||
           n == "matrix_multiplication_stress" ||
           n == "cryptographic_hash_loop" ||
           n == "prime_number_generator" ||
           n == "floating_point_intensive_task" ||
           n == "recursive_compute_benchmark" ||
           n == "multi_thread_cpu_saturation" ||
           n == "vectorized_compute_stress" ||
           n == "cache_thrashing_compute_task" ||
           n == "arithmetic_pipeline_saturation";
}

bool isBurst(const std::string& n) {
    return n == "burst" ||
           n == "burst_compute_idle_cycle" ||
           n == "random_cpu_spike_generator" ||
           n == "intermittent_heavy_processing" ||
           n == "burst_sleep_compute_pattern" ||
           n == "stochastic_execution_behavior" ||
           n == "jittered_cpu_activity" ||
           n == "irregular_load_generator" ||
           n == "periodic_burst_decay_model";
}

bool isScheduler(const std::string& n) {
    return n == "scheduler" ||
           n == "thread_pool_switch_storm" ||
           n == "cooperative_yield_flood" ||
           n == "rapid_spawn_join_cycles" ||
           n == "scheduler_ping_pong_threads" ||
           n == "preemption_heavy_execution" ||
           n == "fairness_stress_simulation" ||
           n == "time_slice_fragmentation_test" ||
           n == "cpu_sharing_contention_model";
}


void safe_cpu_task(uint64_t end_ns) {
    while (get_mono_ns() < end_ns) {
        volatile double x = 0;

        for (int i = 0; i < config.cpu_step_limit; i++) {
            x += std::sqrt(i * 3.14159);
        }

        std::this_thread::sleep_for(
            std::chrono::microseconds(config.sleep_us)
        );
    }
}


void safe_burst_task(uint64_t end_ns) {

    while (get_mono_ns() < end_ns) {

        uint64_t burst_end =
            get_mono_ns() + 3 * 1000000ULL;

        while (get_mono_ns() < burst_end &&
               get_mono_ns() < end_ns) {

            volatile int y = 0;

            for (int i = 0; i < 800; i++) {
                y += i;
            }
        }

        std::this_thread::sleep_for(
            std::chrono::milliseconds(20)
        );
    }
}


void safe_scheduler_task(uint64_t end_ns) {

    int threads = config.max_threads;
    std::vector<std::thread> workers;

    for (int i = 0; i < threads; i++) {

        workers.emplace_back([&, i]() {

            while (get_mono_ns() < end_ns) {

                if (i % 2 == 0) {
                    std::this_thread::yield();
                } else {
                    volatile int z = 0;

                    for (int j = 0; j < 1500; j++) {
                        z += j;
                    }
                }

                std::this_thread::sleep_for(
                    std::chrono::microseconds(100)
                );
            }
        });
    }

    for (auto& t : workers) {
        t.join();
    }
}