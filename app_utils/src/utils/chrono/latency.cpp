#include <iostream>
#include <utility>

#include "latency.h"
#include "time.h"

utils::Latency::Latency(std::string functionName)
    : start(utils::Time::getEpocTimeInNanoseconds()),
      functionName(std::move(functionName))
{
}

utils::Latency::~Latency()
{
    const auto elapsed = utils::Time::getEpocTimeInNanoseconds() - start;
    std::clog << functionName << " took: " << elapsed << " nanoseconds" << std::endl;
}
