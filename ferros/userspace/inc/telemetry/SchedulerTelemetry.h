#pragma once

#include "ITelemetry.h"
#include "events.h"
#include <cstring>

class SchedulerTelemetry final : public ITelemetry {
public:
    SchedulerTelemetry() = default;
    explicit SchedulerTelemetry(const foc_event& ev) : event(ev) {}

    SchedulerTelemetry(SchedulerTelemetry&&) = default;
    SchedulerTelemetry& operator=(SchedulerTelemetry&&) = default;

    TelemetryType type() const noexcept override { return TelemetryType::SchedulerSwitch; }
    std::string typeString() const noexcept override { return "SchedulerSwitch"; }
    
    uint64_t timestamp() const noexcept override { return event.h.ts; }

    uint32_t pid() const noexcept override { return event.p.sw.next_pid; }
    uint32_t tgid() const noexcept override { return event.h.tgid; }
    uint32_t ppid() const noexcept override { return 0; } // Unavailable in sched_switch
    uint32_t cpu() const noexcept override { return event.h.cpu; }
    
    std::string comm() const noexcept override {
        return std::string(event.p.sw.next_comm, strnlen(event.p.sw.next_comm, 16));
    }

    const foc_event& getEvent() const noexcept { return event; }

private:
    foc_event event;
};
