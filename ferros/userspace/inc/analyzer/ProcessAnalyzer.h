#pragma once
#include "analyzer/IAnalyzer.h"
#include "telemetry/ProcessForkTelemetry.h"
#include "telemetry/ProcessExitTelemetry.h"
#include "insight/InsightFactory.h"

#include <unordered_map>
#include <deque>
#include <vector>

class ProcessAnalyzer final : public IAnalyzer {
    struct ProcessNode {
        uint32_t parent = 0;
        std::vector<uint32_t> children;
        uint64_t fork_time = 0;
        uint64_t exit_time = 0;
    };

    std::unordered_map<uint32_t, ProcessNode> process_tree;
    std::deque<uint64_t> fork_window;
    std::deque<uint64_t> exit_window;
    uint64_t window_ns = 1000000000ULL; // 1 second
    
    size_t prev_fork_count = 1;
    uint64_t last_eval = 0;

public:
    bool supports(TelemetryType type) const override {
        return type == TelemetryType::ProcessFork || type == TelemetryType::ProcessExit;
    }

    std::vector<std::unique_ptr<IInsight>> analyze(const ITelemetry& telemetry) override {
        std::vector<std::unique_ptr<IInsight>> insights;
        uint64_t now = telemetry.timestamp();

        if (telemetry.type() == TelemetryType::ProcessFork) {
            auto& ft = static_cast<const ProcessForkTelemetry&>(telemetry);
            uint32_t parent = ft.getEvent().p.fk.parent_pid;
            uint32_t child = ft.getEvent().p.fk.child_pid;

            auto& node = process_tree[child];
            node.parent = parent;
            node.fork_time = now;
            process_tree[parent].children.push_back(child);

            fork_window.push_back(now);
        } 
        else if (telemetry.type() == TelemetryType::ProcessExit) {
            auto& et = static_cast<const ProcessExitTelemetry&>(telemetry);
            uint32_t pid = et.getEvent().p.ex.pid;
            
            auto& node = process_tree[pid];
            node.exit_time = now;
            exit_window.push_back(now);

            if (node.fork_time > 0) {
                uint64_t lifetime = now - node.fork_time;
                insights.push_back(InsightFactory::createProcessInsight("ProcessLifecycle", telemetry, node.fork_time, nlohmann::json{
                    {"pid", pid},
                    {"lifetime_ns", lifetime}
                }));
            }
        } else {
            return insights;
        }

        while (!fork_window.empty() && (now - fork_window.front()) > window_ns) fork_window.pop_front();
        while (!exit_window.empty() && (now - exit_window.front()) > window_ns) exit_window.pop_front();

        size_t current_forks = fork_window.size();
        size_t current_exits = exit_window.size();

        if (current_forks > 50) {
            insights.push_back(InsightFactory::createGlobalInsight("ForkBurst", telemetry, nlohmann::json{
                {"forks_in_window", current_forks}
            }));
        }

        double growth = (double)current_forks / (prev_fork_count > 0 ? prev_fork_count : 1);
        if (growth > 2.5 && current_forks > 10) {
            insights.push_back(InsightFactory::createGlobalInsight("ProcessExplosion", telemetry, nlohmann::json{
                {"growth_factor", growth},
                {"current_forks", current_forks}
            }));
        }
        
        if (now - last_eval > window_ns / 2) {
            prev_fork_count = current_forks;
            last_eval = now;
            
            double churn = current_forks + current_exits;
            insights.push_back(InsightFactory::createGlobalInsight("ProcessChurn", telemetry, nlohmann::json{
                {"churn", churn}
            }));
            
            int zombies = 0;
            for (const auto& pair : process_tree) {
                if (pair.second.exit_time == 0 && pair.second.fork_time > 0 && (now - pair.second.fork_time) > 60000000000ULL) {
                    zombies++;
                }
            }
            if (zombies > 0) {
                insights.push_back(InsightFactory::createGlobalInsight("ZombieProcess", telemetry, nlohmann::json{
                    {"potential_zombies", zombies}
                }));
            }
        }

        return insights;
    }
};
