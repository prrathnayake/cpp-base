#include <iostream>
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <thread>
#include <unordered_map>

namespace utils
{
    class ThreadPool
    {
    public:
        ThreadPool() : stop(false) {}
        void addThread(const std::string &threadName);
        void addTask(const std::string &threadName, std::function<void()> task);
        void joinAll();

    private:
        std::unordered_map<std::string, std::thread> threads;
        std::unordered_map<std::string, std::queue<std::function<void()>>> tasks;
        std::unordered_map<std::string, std::mutex> taskMutexes;
        std::unordered_map<std::string, std::condition_variable> taskCVs;
        bool stop;

        void threadFunc(const std::string &threadName);
    };
}
