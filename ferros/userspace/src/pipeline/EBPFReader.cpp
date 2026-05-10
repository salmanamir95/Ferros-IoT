#include "pipeline/EBPFReader.h"
#include "telemetry/TelemetryFactory.h"
#include "events.h"
#include <iostream>

EBPFReader::EBPFReader(Pipeline& pipeline) 
    : pipeline(pipeline), skel(nullptr), rb(nullptr), running(false) {}

EBPFReader::~EBPFReader() {
    stop();
}

bool EBPFReader::start() {
    skel = cpu_usage_bpf__open_and_load();
    if (!skel) {
        std::cerr << "Failed to open and load BPF skeleton\n";
        return false;
    }

    int err = cpu_usage_bpf__attach(skel);
    if (err) {
        std::cerr << "Failed to attach BPF skeleton\n";
        cpu_usage_bpf__destroy(skel);
        skel = nullptr;
        return false;
    }

    rb = ring_buffer__new(bpf_map__fd(skel->maps.events), handle_event, this, nullptr);
    if (!rb) {
        std::cerr << "Failed to create ring buffer\n";
        cpu_usage_bpf__destroy(skel);
        skel = nullptr;
        return false;
    }

    running = true;
    while (running) {
        err = ring_buffer__poll(rb, 100 /* timeout, ms */);
        if (err < 0 && err != -EINTR) {
            std::cerr << "Error polling ring buffer: " << err << "\n";
            break;
        }
    }

    return true;
}

void EBPFReader::stop() {
    running = false;
    if (rb) {
        ring_buffer__free(rb);
        rb = nullptr;
    }
    if (skel) {
        cpu_usage_bpf__destroy(skel);
        skel = nullptr;
    }
}

int EBPFReader::handle_event(void* ctx, void* data, size_t size) {
    auto* reader = static_cast<EBPFReader*>(ctx);
    auto* e = static_cast<foc_event*>(data);

    auto telemetry = TelemetryFactory::create(*e);
    if (telemetry) {
        reader->pipeline.process(std::move(telemetry));
    }

    return 0;
}
