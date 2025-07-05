#include "ThreadPool.h"

using namespace utils;

std::unique_ptr<ThreadPool> ThreadPool::instance_;
std::once_flag ThreadPool::initFlag;

ThreadPool& ThreadPool::instance(size_t threadCount)
{
    std::call_once(initFlag, [&]() {
        instance_.reset(new ThreadPool(threadCount));  // ✅ works inside the class
    });

    return *instance_;
}

ThreadPool::ThreadPool(size_t numThreads) : stop(false)
{
    if (numThreads == 0)
    {
        unsigned int hardwareThreads = std::thread::hardware_concurrency();
        numThreads = std::max(1u, hardwareThreads > 1 ? hardwareThreads - 1 : 1);
    }

    std::clog << "[ThreadPool] Starting with " << numThreads << " threads.\n";

    for (size_t i = 0; i < numThreads; ++i)
    {
        workers.emplace_back([this]() { this->workerThread(); });
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }

    condition.notify_all();

    for (std::thread& worker : workers)
    {
        if (worker.joinable())
            worker.join();
    }

    std::clog << "[ThreadPool] Shutdown complete.\n";
}

void ThreadPool::workerThread()
{
    while (true)
    {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this]() { return stop || !tasks.empty(); });

            if (stop && tasks.empty())
                return;

            task = std::move(tasks.front());
            tasks.pop();
        }

        try
        {
            task();
        }
        catch (const std::exception& e)
        {
            std::clog << "[ThreadPool] Exception in task: " << e.what() << '\n';
        }
        catch (...)
        {
            std::clog << "[ThreadPool] Unknown exception in task.\n";
        }
    }
}
