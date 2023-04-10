#include <iostream>

#include "latency.h"
#include "time.h"

utils::Latency::Latency(std::string functionName)
{
    this->functionName = functionName;
    start = utils::Time::getEpocTimeInNanoseconds();
}
utils::Latency::~Latency()
{
    std::cout << functionName << " took : " << utils::Time::getEpocTimeInNanoseconds() - start << "nanoseconds" << std::endl;
}
