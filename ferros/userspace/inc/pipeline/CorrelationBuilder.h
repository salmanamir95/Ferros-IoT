#pragma once
#include <string>

class CorrelationBuilder {
public:
    static std::string build(uint32_t pid, uint32_t tgid, uint64_t fork_ts, const std::string& comm) {
        return "p:" + std::to_string(pid) + 
               ":t:" + std::to_string(tgid) + 
               ":f:" + std::to_string(fork_ts) + 
               ":c:" + comm;
    }
    
    static std::string buildGlobal(uint64_t ts) {
        return "sys_global_" + std::to_string(ts);
    }
};
