#include <iostream>
#include <chrono>
#include <ctime>

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
    time_t now = time(0);
    tm *ltm = localtime(&now);

    std::cout << "Year:" << 1900 + ltm->tm_year << std::endl;
    std::cout << "Month: " << 1 + ltm->tm_mon << std::endl;
    std::cout << "Day: " << ltm->tm_mday << std::endl;
    std::cout << "Time: " << 5 + ltm->tm_hour << ":";
    std::cout << 30 + ltm->tm_min << ":";
    std::cout << ltm->tm_sec << std::endl;
}

void utils::Time::holdSeconds(int secs)
{
    long long int pre = utils::Time::getEpocTimeInMilliseconds();
    bool hold = true;
    while (hold)
    {
        long long int now = utils::Time::getEpocTimeInMilliseconds();
        if (now == (pre + (secs * 1000)))
        {
            hold = false;
        }
    }
}

void utils::Time::holdMiliseconds(int miliseconds)
{
    long long int pre = utils::Time::getEpocTimeInMicroseconds();
    bool hold = true;
    while (hold)
    {
        long long int now = utils::Time::getEpocTimeInMicroseconds();
        if (now == (pre + (miliseconds * 1000)))
        {
            hold = false;
        }
    }
}

std::string utils::Time::validTime(int i)
{
    std::string str;
    if (i < 10)
    {
        return str = "00" + std::to_string(i);
    }
    else if (i < 100)
    {
        return str = "0" + std::to_string(i);
    }
    else
    {
        return str = std::to_string(i);
    }
}

std::string utils::Time::logTime()
{
    time_t now = time(0);
    struct tm tstruct;
    char log[80];
    tstruct = *localtime(&now);

    strftime(log, sizeof(log), "%Y-%m-%d %T", &tstruct);

    int mili = getEpocTimeInMilliseconds() % 1000;
    int micro = getEpocTimeInMicroseconds() % 1000;
    int nano = getEpocTimeInNanoseconds() % 1000;

    std::string time = "[" + std::string(log) + ":" + validTime(mili) + ":" + validTime(micro) + ":" + validTime(nano) + "]";

    return time;
}