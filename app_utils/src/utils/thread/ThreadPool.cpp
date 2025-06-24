#include "ThreadPool.h"

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <atomic>
#include <iostream>

utils::ThreadPool::ThreadPool() : stop(false)
{
    unsigned int hardwareThreads = std::thread::hardware_concurrency();
    if (hardwareThreads == 0)
        hardwareThreads = 2;

    size_t numThreads = std::max(1u, hardwareThreads - 1);
    std::cout << "Starting thread pool with " << numThreads << " threads.\n";

    for (size_t i = 0; i < numThreads; ++i)
    {
        workers.emplace_back([this]()
                             { this->workerThread(); });
    }
}

utils::ThreadPool::ThreadPool(size_t numThreads) : stop(false)
{
    unsigned int maxThreads = std::thread::hardware_concurrency();
    if (maxThreads == 0)
        maxThreads = 2;

    if (numThreads > maxThreads)
    {
        std::cout << "[Warning] Requested " << numThreads
                  << " threads, but system supports a maximum of " << maxThreads
                  << ". Using " << maxThreads << " threads instead.\n";
        numThreads = maxThreads;
    }

    std::cout << "Starting thread pool with " << numThreads << " threads.\n";
    for (size_t i = 0; i < numThreads; ++i)
    {
        workers.emplace_back([this]()
                             { this->workerThread(); });
    }
}

utils::ThreadPool::~ThreadPool()
{
    stop = true;
    condition.notify_all();

    for (std::thread &worker : workers)
    {
        if (worker.joinable())
            worker.join();
    }
}

void utils::ThreadPool::workerThread()
{
    while (true)
    {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this]()
                           { return stop || !tasks.empty(); });

            if (stop && tasks.empty())
                return;

            task = std::move(tasks.front());
            tasks.pop();
        }

        task();
    }
}
