#include <iostream>
#include "ThreadPool.h"

void utils::ThreadPool::addThread(const std::string &threadName)
{
    if (threads.count(threadName) > 0)
    {
        std::cerr << "Thread with name " << threadName << " already exists.\n";
        return;
    }

    threads[threadName] = std::thread(&ThreadPool::threadFunc, this, threadName);
}

void utils::ThreadPool::addTask(const std::string &threadName, std::function<void()> task)
{
    if (threads.count(threadName) == 0)
    {
        std::cerr << "Thread " << threadName << " does not exist.\n";
        return;
    }

    std::unique_lock<std::mutex> lock(taskMutexes[threadName]);
    if (tasks[threadName].size() > 0)
    {
        std::cerr << "Thread " << threadName << " is already executing a task.\n";
        return;
    }

    tasks[threadName].push(task);
    taskCVs[threadName].notify_one();
}

void utils::ThreadPool::joinAll()
{
    stop = true;
    std::cerr << "Current Thread count -  " << threads.size() << std::endl;
    for (auto &pair : threads)
    {
        if (tasks[pair.first].size() > 0)
        {
            std::cerr << "Thread " << pair.first << " is already executing a task.\n";
            return;
        }
        if (pair.second.joinable())
        {
            std::cout << "Join Thread " << pair.first << std::endl;
            pair.second.join();
        }
        std::cout << "joined" << std::endl;
    }
}

void utils::ThreadPool::threadFunc(const std::string &threadName)
{
    while (!stop)
    {
        std::unique_lock<std::mutex> lock(taskMutexes[threadName]);
        while (tasks[threadName].empty() && !stop)
        {
            taskCVs[threadName].wait(lock);
        }

        if (tasks[threadName].empty())
        {
            continue;
        }

        auto task = tasks[threadName].front();
        tasks[threadName].pop();
        lock.unlock();

        try
        {
            task();
        }
        catch (const std::exception &e)
        {
            std::cerr << "Exception caught in thread " << threadName << ": " << e.what() << std::endl;
        }
    }
}
