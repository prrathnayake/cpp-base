#include "fileLogger.h"
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <filesystem>
#include <sys/file.h> // for flock

#include "fileLogger.h"
#include "../chrono/time.h"

namespace fs = std::filesystem;

std::mutex utils::FileLogger::logMutex;

utils::FileLogger::FileLogger(const std::string &path)
    : logFilePath(path)
{
    fs::path logPath(logFilePath);
    if (logPath.has_parent_path())
    {
        fs::create_directories(logPath.parent_path());
    }
}

utils::FileLogger::~FileLogger() {}

void utils::FileLogger::log(MessageCode level, const std::string &message)
{
    print(levelToString(level), message);
}

void utils::FileLogger::logMeta(MessageCode level, const std::string &message,
                                const char *file, int line, const char *func)
{
    std::string metadata = std::string(file) + ":" + std::to_string(line) +
                           " in " + func;
    std::string full_message = utils::Time::logTime() +" [" + levelToString(level) + "] " + metadata + " | " + message;
    print(levelToString(level), full_message);
}

void utils::FileLogger::print(const std::string &levelStr, const std::string &message)
{
    std::lock_guard<std::mutex> lock(logMutex);

    int fd = open(logFilePath.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1)
    {
        perror("open");
        return;
    }

    // Lock the file (blocking)
    if (flock(fd, LOCK_EX) == -1)
    {
        perror("flock");
        close(fd);
        return;
    }

    std::string full_message = message + "\n";
    if (write(fd, full_message.c_str(), full_message.size()) == -1)
    {
        perror("write");
    }

    // Unlock the file
    flock(fd, LOCK_UN);

    close(fd);
}

std::string utils::FileLogger::levelToString(MessageCode level)
{
    switch (level)
    {
    case INFO:
        return "INFO";
    case ERROR:
        return "ERROR";
    case WARN:
        return "WARN";
    case DEBUG:
        return "DEBUG";
    default:
        return "UNKNOWN";
    }
}
