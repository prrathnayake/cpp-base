#include <iostream>
#include <fstream>

#include "../chrono/time.h"
#include "consoleLogger.h"

void utils::ConsoleLogger::info(std::string message)
{
    std::cout << utils::Time::logTime() << " [INFO] " << message << "\n";
}