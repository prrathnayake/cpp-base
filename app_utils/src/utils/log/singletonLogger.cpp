#include "singletonLogger.h"
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <filesystem>
#include <sys/file.h>

#include "../chrono/time.h"

namespace fs = std::filesystem;

std::mutex utils::SingletonLogger::logMutex;

utils::SingletonLogger &utils::SingletonLogger::instance(const std::string &logFilePath)
{
    static SingletonLogger instance(logFilePath);
    return instance;
}

utils::SingletonLogger::SingletonLogger(const std::string &path)
    : logFilePath(path)
{
    fs::path logPath(logFilePath);
    if (fs::exists(logPath))
    {
        fs::remove(logPath);
    }

    if (logPath.has_parent_path())
    {
        fs::create_directories(logPath.parent_path());
    }
}

utils::SingletonLogger::~SingletonLogger() = default;

// Helper to convert enum to string
static std::string messageCodeToString( utils::SingletonLogger::MessageCode level)
{
    switch (level)
    {
    case utils::SingletonLogger::MessageCode::ERROR:
        return "ERROR";
    case utils::SingletonLogger::MessageCode::WARNING:
        return "WARNING";
    case utils::SingletonLogger::MessageCode::INFO:
        return "INFO";
    case utils::SingletonLogger::MessageCode::DEBUG:
        return "DEBUG";
    default:
        return "UNKNOWN";
    }
}

void utils::SingletonLogger::log(MessageCode level, const std::string &message)
{
    std::string levelStr = messageCodeToString(level);
    std::string timestamp = utils::Time::logTime();
    std::string full_message = timestamp + " [ " + levelStr + " ] " + message;
    printLog(full_message);
    printConsole(level, full_message);
}

void utils::SingletonLogger::logMeta(MessageCode level, const std::string &message,
                                     const char *file, int line, const char *func)
{
    std::string metadata = std::string(file) + ":" + std::to_string(line) + " in " + func;
    std::string levelStr = messageCodeToString(level);
    std::string timestamp = utils::Time::logTime();
    std::string full_message = timestamp + " [ " + levelStr + " ] " + message + " | " + metadata;
    printLog(full_message);
    printConsole(level, full_message);
}

void utils::SingletonLogger::printLog(const std::string &message)
{
    std::lock_guard<std::mutex> lock(logMutex);

    int fd = open(logFilePath.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1)
    {
        perror("open");
        return;
    }

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

    flock(fd, LOCK_UN);
    close(fd);
}

void utils::SingletonLogger::printConsole(utils::SingletonLogger::MessageCode level, const std::string &message)
{
#ifdef DEBUG_MODE
    bool debugEnabled = true;
#else
    bool debugEnabled = false;
#endif

    switch (level)
    {
    case utils::SingletonLogger::MessageCode::ERROR:
        std::cout << "\033[1;31m[ERROR] " << message << "\033[0m" << std::endl;
        break;
    case utils::SingletonLogger::MessageCode::WARNING:
        std::cout << "\033[1;33m[WARNING] " << message << "\033[0m" << std::endl;
        break;
    case utils::SingletonLogger::MessageCode::INFO:
        std::cout << "[INFO] " << message << std::endl;
        break;
    case utils::SingletonLogger::MessageCode::DEBUG:
        if (debugEnabled)
            std::cout << "\033[1;36m[DEBUG] " << message << "\033[0m" << std::endl;
        break;
    default:
        std::cout << message << std::endl;
        break;
    }
}