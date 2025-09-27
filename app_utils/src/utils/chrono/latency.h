#pragma once
#include <cstdint>
#include <string>

namespace utils {
    class Latency {
    public:
        uint64_t start;
        std::string functionName;
        explicit Latency(std::string functionName);
        ~Latency();
    };
}
