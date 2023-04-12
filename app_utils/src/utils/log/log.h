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
            void printLog(std::string message);
    };
}