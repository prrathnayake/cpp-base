#include <iostream>

#include "../chrono/time.h"
#include "consoleLogger.h"

void utils::ConsoleLogger::info(const std::string &message)
{
    std::cout << utils::Time::logTime() << " [INFO] " << message << "\n";
}
