#pragma once

#include "ITelemetry.h"
#include "events.h"
#include <cstring>

class ProcessForkTelemetry final : public ITelemetry {
public:
    ProcessForkTelemetry() = default;
    explicit ProcessForkTelemetry(const foc_event& ev) : event(ev) {}

    ProcessForkTelemetry(ProcessForkTelemetry&&) = default;
    ProcessForkTelemetry& operator=(ProcessForkTelemetry&&) = default;

    TelemetryType type() const noexcept override { return TelemetryType::ProcessFork; }
    std::string typeString() const noexcept override { return "ProcessFork"; }
    
    uint64_t timestamp() const noexcept override { return event.h.ts; }

    uint32_t pid() const noexcept override { return event.p.fk.child_pid; }
    uint32_t tgid() const noexcept override { return event.p.fk.child_tgid; }
    uint32_t ppid() const noexcept override { return event.p.fk.parent_pid; }
    uint32_t cpu() const noexcept override { return event.h.cpu; }
    
    std::string comm() const noexcept override {
        return std::string(event.p.fk.child_comm, strnlen(event.p.fk.child_comm, 16));
    }

    const foc_event& getEvent() const noexcept { return event; }

private:
    foc_event event;
};
