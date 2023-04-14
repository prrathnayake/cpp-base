#pragma one
#include <iostream>
#include <fstream>

namespace utils{
    class FileLogger: public Logger{
        private:
            std::fstream logFile;
        public:
            FileLogger();
            ~FileLogger();
            void info(std::string message);
    };
}