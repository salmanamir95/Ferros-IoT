#pragma once

#include <memory>
#include "events.h"
#include "telemetry/ITelemetry.h"
#include "telemetry/SchedulerTelemetry.h"
#include "telemetry/ProcessForkTelemetry.h"
#include "telemetry/ProcessExitTelemetry.h"

class TelemetryFactory {
public:
    static std::unique_ptr<ITelemetry> create(const foc_event& e) {
        switch (e.h.type) {
            case EVENT_SCHED_SWITCH:
                return std::make_unique<SchedulerTelemetry>(e);
            case EVENT_PROCESS_FORK:
                return std::make_unique<ProcessForkTelemetry>(e);
            case EVENT_PROCESS_EXIT:
                return std::make_unique<ProcessExitTelemetry>(e);
            default:
                return nullptr;
        }
    }
};
