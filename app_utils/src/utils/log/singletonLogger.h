#pragma once

#include <string>
#include <mutex>

namespace utils
{
    class SingletonLogger
    {
    public:
        // Deleted copy/move constructors and assignments
        SingletonLogger(const SingletonLogger&) = delete;
        SingletonLogger& operator=(const SingletonLogger&) = delete;
        SingletonLogger(SingletonLogger&&) = delete;
        SingletonLogger& operator=(SingletonLogger&&) = delete;

        // Get singleton instance
        static SingletonLogger& instance(const std::string& logFilePath = "log/log.txt");

        enum MessageCode
        {
            INFO = 0,
            ERROR = 1,
            WARN = 2,
            DEBUG = 3
        };

        void log(MessageCode level, const std::string& message);
        void logMeta(MessageCode level, const std::string& message,
                     const char* file, int line, const char* func);

    private:
        explicit SingletonLogger(const std::string& logFilePath);
        ~SingletonLogger();

        void printLog(const std::string& message);
        void printConsole(const std::string& message);

        static std::mutex logMutex;
        std::string logFilePath;
    };
}
