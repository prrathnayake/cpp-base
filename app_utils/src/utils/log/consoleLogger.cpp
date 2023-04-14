#include <iostream>
#include <fstream>

#include "../index.h"

void utils::ConsoleLogger::info(std::string message)
{
    std::cout << utils::Time::logTime() << " [INFO] " << message << "\n";
}