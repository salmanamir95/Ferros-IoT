#pragma once

#include <memory>
#include "pipeline/Pipeline.h"
#include "cpu_usage.skel.h"
#include <bpf/libbpf.h>

class EBPFReader {
public:
    explicit EBPFReader(Pipeline& pipeline);
    ~EBPFReader();

    bool start();
    void stop();

private:
    Pipeline& pipeline;
    struct cpu_usage_bpf* skel;
    struct ring_buffer* rb;
    bool running;

    static int handle_event(void* ctx, void* data, size_t size);
};
