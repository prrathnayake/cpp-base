#pragma one
#include <iostream>
#include <fstream>

namespace utils{
    class Logger{
        public:
            virtual void info(std::string message) = 0;
    };
}