#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <filesystem>
#include <mutex>

#include "../index.h" // or wherever FileLogger is declared

namespace fs = std::filesystem;
std::mutex utils::FileLogger::logMutex;

utils::FileLogger::FileLogger(const std::string& path)
    : logFilePath(path)
{
    fs::path logPath(logFilePath);
    if (fs::exists(logFilePath))
    {
        fs::remove(logFilePath);
    }

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
    std::string metadata = " => " + std::string(file) + ":" + std::to_string(line) +
                           " in " + func;
    std::string full_message = utils::Time::logTime() + " [" + levelToString(level) + "] " +
                               metadata + " | " + message;
    print(levelToString(level), full_message);
}

void utils::FileLogger::print(const std::string &levelStr, const std::string &message)
{
    std::lock_guard<std::mutex> lock(logMutex); // thread-safe

    int fd = open(logFilePath.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1)
    {
        perror("open");
        return;
    }

    struct flock fl{};
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;

    fcntl(fd, F_SETLKW, &fl); // lock the file

    std::string full_message = utils::Time::logTime() + " [" + levelStr + "] " + message + "\n";
    if (write(fd, full_message.c_str(), full_message.size()) == -1)
    {
        perror("write");
    }

    fl.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &fl); // unlock

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