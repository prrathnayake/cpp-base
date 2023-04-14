#include <iostream>
#include <fstream>

#include "../index.h"

utils::Log::Log()
{
    logFile.open("log.txt", std::ios::out);
}

utils::Log::~Log()
{
    logFile.close();
}

void utils::Log::printLog(std::string message)
{
    logFile << utils::Time::logTime() << " \n";
}