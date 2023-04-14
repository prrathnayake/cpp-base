#pragma one
#include <iostream>
#include <fstream>

namespace utils
{
    class ConsoleLogger : public Logger
    {
    public:
        void info(std::string message);
    };
}