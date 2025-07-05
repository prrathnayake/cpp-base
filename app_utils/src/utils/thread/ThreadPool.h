#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <atomic>
#include <iostream>

namespace utils
{
    class ThreadPool
    {
    public:
        // Singleton with optional thread count
        static ThreadPool &instance(size_t threadCount = 0);

        // Deleted copy/move
        ThreadPool(const ThreadPool &) = delete;
        ThreadPool &operator=(const ThreadPool &) = delete;

        ~ThreadPool();

        template <class F, class... Args>
        auto enqueue(F &&f, Args &&...args)
            -> std::future<typename std::invoke_result<F, Args...>::type>;

    private:
        explicit ThreadPool(size_t numThreads = 0); // Default or user-defined
        void workerThread();

        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;

        std::mutex queueMutex;
        std::condition_variable condition;
        std::atomic<bool> stop;

        static std::unique_ptr<ThreadPool> instance_;
        static std::once_flag initFlag;
    };

    // Enqueue implementation
    template <class F, class... Args>
    auto ThreadPool::enqueue(F &&f, Args &&...args)
        -> std::future<typename std::invoke_result<F, Args...>::type>
    {
        using return_type = typename std::invoke_result<F, Args...>::type;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<return_type> result = task->get_future();

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            if (stop)
                throw std::runtime_error("enqueue on stopped ThreadPool");

            tasks.emplace([task]()
                          { (*task)(); });
        }

        condition.notify_one();
        return result;
    }
}
