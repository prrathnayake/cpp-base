#pragma once
#include <iostream>
#include <chrono>

namespace utils
{
    class Time
    {
    private:
        static std::string validTime(int i);

    public:
        static uint64_t getEpocTimeInNanoseconds();
        static uint64_t getEpocTimeInMicroseconds();
        static uint64_t getEpocTimeInMilliseconds();
        static uint64_t getEpocTimeInSeconds();
        static uint64_t getEpocTimeInMinutes();
        static void printNowTime();
        static void holdSeconds(int secs);
        static void holdMiliseconds(int miliseconds);
        static std::string logTime();
    };
}