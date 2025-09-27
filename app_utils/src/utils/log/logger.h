#pragma once
#include <string>

namespace utils {
    class Logger {
    public:
        virtual ~Logger() = default;
        virtual void info(const std::string &message) = 0;
    };
}
