#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>

#include "time.h"

uint64_t utils::Time::getEpocTimeInNanoseconds()
{
    return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

uint64_t utils::Time::getEpocTimeInMicroseconds()
{
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

uint64_t utils::Time::getEpocTimeInMilliseconds()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

uint64_t utils::Time::getEpocTimeInSeconds()
{
    return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

uint64_t utils::Time::getEpocTimeInMinutes()
{
    return std::chrono::duration_cast<std::chrono::minutes>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void utils::Time::printNowTime()
{
    const auto now = std::chrono::system_clock::now();
    const std::time_t timeValue = std::chrono::system_clock::to_time_t(now);

    std::tm tmSnapshot{};
#if defined(_WIN32)
    localtime_s(&tmSnapshot, &timeValue);
#else
    localtime_r(&timeValue, &tmSnapshot);
#endif

    std::cout << std::put_time(&tmSnapshot, "%Y-%m-%d %H:%M:%S") << std::endl;
}

void utils::Time::holdSeconds(int secs)
{
    if (secs <= 0)
    {
        return;
    }

    std::this_thread::sleep_for(std::chrono::seconds(secs));
}

void utils::Time::holdMiliseconds(int miliseconds)
{
    if (miliseconds <= 0)
    {
        return;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(miliseconds));
}

std::string utils::Time::validTime(int i)
{
    if (i < 0)
    {
        i = 0;
    }

    std::ostringstream stream;
    stream << std::setfill('0') << std::setw(3) << i;
    return stream.str();
}

std::string utils::Time::logTime()
{
    const auto now = std::chrono::system_clock::now();
    const std::time_t timeValue = std::chrono::system_clock::to_time_t(now);

    std::tm tmSnapshot{};
#if defined(_WIN32)
    localtime_s(&tmSnapshot, &timeValue);
#else
    localtime_r(&timeValue, &tmSnapshot);
#endif

    std::ostringstream logStream;
    logStream << std::put_time(&tmSnapshot, "%Y-%m-%d %T");

    const auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch());
    const auto nanosCount = nanos.count();
    const int milli = static_cast<int>((nanosCount / 1'000'000) % 1000);
    const int micro = static_cast<int>((nanosCount / 1'000) % 1000);
    const int nano = static_cast<int>(nanosCount % 1000);

    std::ostringstream result;
    result << '[' << logStream.str() << ':' << validTime(milli) << ':' << validTime(micro) << ':' << validTime(nano) << ']';
    return result.str();
}