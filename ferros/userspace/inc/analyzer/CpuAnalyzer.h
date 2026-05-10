#pragma once
#include "analyzer/IAnalyzer.h"
#include "telemetry/SchedulerTelemetry.h"
#include "insight/InsightFactory.h"

#include <deque>
#include <unordered_map>
#include <cmath>

class CpuAnalyzer final : public IAnalyzer {
    struct WindowEvent {
        uint64_t ts;
        uint32_t pid;
        bool migrated;
    };

    struct CpuState {
        std::deque<WindowEvent> window;
        std::unordered_map<uint32_t, int> pid_switch_count;
        int total_migrations = 0;
        uint64_t event_counter = 0;
    };

    std::unordered_map<uint32_t, CpuState> cpu_states;
    uint64_t window_ns = 1000000000ULL; // 1 second

public:
    bool supports(TelemetryType type) const override {
        return type == TelemetryType::SchedulerSwitch;
    }

    std::vector<std::unique_ptr<IInsight>> analyze(const ITelemetry& telemetry) override {
        std::vector<std::unique_ptr<IInsight>> insights;
        if (telemetry.type() != TelemetryType::SchedulerSwitch) return insights;

        auto& st = static_cast<const SchedulerTelemetry&>(telemetry);
        auto& event = st.getEvent();
        uint32_t cpu = event.h.cpu;
        uint64_t now = event.h.ts;
        uint32_t next_pid = event.p.sw.next_pid;
        bool migrated = (event.p.sw.prev_cpu != cpu);

        auto& state = cpu_states[cpu];
        state.window.push_back({now, next_pid, migrated});
        state.pid_switch_count[next_pid]++;
        if (migrated) state.total_migrations++;
        state.event_counter++;

        while (!state.window.empty() && (now - state.window.front().ts) > window_ns) {
            auto& old = state.window.front();
            state.pid_switch_count[old.pid]--;
            if (state.pid_switch_count[old.pid] <= 0) {
                state.pid_switch_count.erase(old.pid);
            }
            if (old.migrated) state.total_migrations--;
            state.window.pop_front();
        }

        double switch_rate = state.window.size(); // Approx 1 second window
        double migration_rate = state.total_migrations;
        
        double entropy = 0.0;
        int max_density = 0;
        uint32_t hot_pid = 0;
        double total_switches = state.window.size();

        if (total_switches > 0) {
            for (const auto& pair : state.pid_switch_count) {
                double p = pair.second / total_switches;
                if (p > 0) entropy -= p * std::log2(p);
                if (pair.second > max_density) {
                    max_density = pair.second;
                    hot_pid = pair.first;
                }
            }
        }

        double pressure_score = (switch_rate * 0.4) + (entropy * 0.4) + (migration_rate * 0.2);

        // Emit insights every 100 events to avoid flooding output
        if (state.event_counter % 100 == 0 && state.window.size() > 0) {
            insights.push_back(InsightFactory::createGlobalInsight("CpuPressure", telemetry, nlohmann::json{
                {"cpu", cpu},
                {"switch_rate", switch_rate},
                {"entropy", entropy},
                {"migration_rate", migration_rate},
                {"pressure_score", pressure_score}
            }));

            insights.push_back(InsightFactory::createGlobalInsight("CpuHotspot", telemetry, nlohmann::json{
                {"cpu", cpu},
                {"hot_pid", hot_pid},
                {"max_density", max_density}
            }));

            insights.push_back(InsightFactory::createGlobalInsight("SchedulingEntropy", telemetry, nlohmann::json{
                {"cpu", cpu},
                {"entropy", entropy}
            }));
        }

        return insights;
    }
};
