#pragma once
#include "analyzer/IAnalyzer.h"
#include "telemetry/SchedulerTelemetry.h"
#include "insight/InsightFactory.h"

#include <deque>
#include <unordered_map>
#include <cmath>

class SchedulerAnalyzer final : public IAnalyzer {
    struct ProcessSchedState {
        uint64_t first_seen_ts = 0;
        uint64_t last_run = 0;
        uint64_t total_runtime = 0;
        int preemption_count = 0;
        int slice_count = 0;
    };

    std::unordered_map<uint32_t, ProcessSchedState> states;
    std::deque<uint64_t> switch_times; // for 100ms burst detection
    uint64_t burst_window_ns = 100000000ULL; // 100ms
    size_t burst_threshold = 500;
    uint64_t event_counter = 0;

public:
    bool supports(TelemetryType type) const override {
        return type == TelemetryType::SchedulerSwitch;
    }

    std::vector<std::unique_ptr<IInsight>> analyze(const ITelemetry& telemetry) override {
        std::vector<std::unique_ptr<IInsight>> insights;
        if (telemetry.type() != TelemetryType::SchedulerSwitch) return insights;

        auto& st = static_cast<const SchedulerTelemetry&>(telemetry);
        auto& event = st.getEvent();
        uint32_t next_pid = event.p.sw.next_pid;
        uint32_t prev_pid = event.p.sw.prev_pid;
        uint64_t now = event.h.ts;

        // Burst tracking
        switch_times.push_back(now);
        while (!switch_times.empty() && (now - switch_times.front()) > burst_window_ns) {
            switch_times.pop_front();
        }

        // State updates
        auto& prev_state = states[prev_pid];
        if (prev_state.last_run > 0 && now >= prev_state.last_run) {
            uint64_t runtime = now - prev_state.last_run;
            prev_state.total_runtime += runtime;
        }
        if (event.p.sw.prev_state == 0) {
            prev_state.preemption_count++;
        }

        auto& next_state = states[next_pid];
        if (next_state.first_seen_ts == 0) {
            next_state.first_seen_ts = now;
        }
        next_state.last_run = now;
        next_state.slice_count++;

        event_counter++;
        if (event_counter % 200 == 0) {
            bool is_burst = (switch_times.size() > burst_threshold);
            if (is_burst) {
                insights.push_back(InsightFactory::createGlobalInsight("SchedulerBurst", telemetry, nlohmann::json{
                    {"burst_switches", switch_times.size()},
                    {"window_ms", 100}
                }));
            }

            double mean_runtime = 0;
            int count = 0;
            for (const auto& pair : states) {
                if (pair.second.total_runtime > 0) {
                    mean_runtime += pair.second.total_runtime;
                    count++;
                }
            }
            double fairness = 1.0;
            if (count > 1) {
                mean_runtime /= count;
                double variance = 0;
                for (const auto& pair : states) {
                    if (pair.second.total_runtime > 0) {
                        double diff = pair.second.total_runtime - mean_runtime;
                        variance += diff * diff;
                    }
                }
                double stddev = std::sqrt(variance / count);
                fairness = 1.0 / (1.0 + (stddev / 1e9)); 
            }

            insights.push_back(InsightFactory::createGlobalInsight("SchedulingFairness", telemetry, nlohmann::json{
                {"fairness_score", fairness},
                {"active_threads", count}
            }));

            if (next_state.total_runtime > 0) {
                double frag = (double)next_state.slice_count / next_state.total_runtime * 1e9;
                insights.push_back(InsightFactory::createProcessInsight("TimeSliceFragmentation", telemetry, next_state.first_seen_ts, nlohmann::json{
                    {"pid", next_pid},
                    {"fragmentation_rate", frag}
                }));
            }
        }

        return insights;
    }
};
