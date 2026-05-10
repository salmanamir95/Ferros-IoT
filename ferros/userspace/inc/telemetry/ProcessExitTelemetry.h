#pragma once

#include "ITelemetry.h"
#include "events.h"
#include <cstring>

class ProcessExitTelemetry final : public ITelemetry {
public:
    ProcessExitTelemetry() = default;
    explicit ProcessExitTelemetry(const foc_event& ev) : event(ev) {}

    ProcessExitTelemetry(ProcessExitTelemetry&&) = default;
    ProcessExitTelemetry& operator=(ProcessExitTelemetry&&) = default;

    TelemetryType type() const noexcept override { return TelemetryType::ProcessExit; }
    std::string typeString() const noexcept override { return "ProcessExit"; }
    
    uint64_t timestamp() const noexcept override { return event.h.ts; }

    uint32_t pid() const noexcept override { return event.p.ex.pid; }
    uint32_t tgid() const noexcept override { return event.p.ex.tgid; }
    uint32_t ppid() const noexcept override { return 0; } // Unavailable in exit
    uint32_t cpu() const noexcept override { return event.p.ex.cpu; }
    
    std::string comm() const noexcept override {
        return std::string(event.p.ex.comm, strnlen(event.p.ex.comm, 16));
    }

    const foc_event& getEvent() const noexcept { return event; }

private:
    foc_event event;
};
