#pragma once
#include <iostream>
#include <cstdint> 

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