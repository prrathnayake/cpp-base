#pragma once
#include <iostream>

namespace utils
{
    class Latency
    {
    public:
        uint64_t start;
        std::string functionName;
        Latency(std::string functionName);
        ~Latency();
    };
}