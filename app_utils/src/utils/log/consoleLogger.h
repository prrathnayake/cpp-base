#pragma once
#include <string>

#include "logger.h"

namespace utils {
    class ConsoleLogger : public Logger {
    public:
        void info(const std::string &message) override;
    };
}
