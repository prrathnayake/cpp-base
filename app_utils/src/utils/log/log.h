#pragma one
#include <iostream>
#include <fstream>

namespace utils{
    class Log{
        private:
            std::fstream logFile;
        public:
            Log();
            ~Log();
            void info(std::string message);
    };
}