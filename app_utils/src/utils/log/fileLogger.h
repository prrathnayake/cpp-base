#pragma once

#include <string>
#include <mutex>

namespace utils
{

    class FileLogger
    {
    public:
        explicit FileLogger(const std::string &logFilePath = "log/log.txt");
        ~FileLogger();

        enum MessageCode
        {
            INFO = 0,
            ERROR = 1,
            WARN = 2,
            DEBUG = 3
        };

        void log(MessageCode level, const std::string &message);
        void logMeta(MessageCode level, const std::string &message,
                     const char *file, int line, const char *func);

    private:
        void print(const std::string &levelStr, const std::string &message);
        static std::string levelToString(MessageCode level);
        std::string logFilePath;
        static std::mutex logMutex;
    };

} // namespace utils

#define LOG(logger, level, msg) \
    (logger).logMeta((level), (msg), __FILE__, __LINE__, __func__)
