#pragma once
#include <iostream>
#include <fstream>

#include "logger.h"

namespace utils
{
    class ConsoleLogger : public Logger
    {
    public:
        void info(std::string message);
    };
}