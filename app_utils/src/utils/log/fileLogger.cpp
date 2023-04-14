#include <iostream>
#include <fstream>

#include "../index.h"

utils::FileLogger::FileLogger()
{
    logFile.open("log.txt", std::ios::out);
}

utils::FileLogger::~FileLogger()
{
    logFile.close();
}

void utils::FileLogger::info(std::string message)
{
    logFile << utils::Time::logTime() << " [INFO] " << message << "\n";
}