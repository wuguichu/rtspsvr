#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

namespace rtspsvr
{
    class ThreadPool
    {
    public:
        ThreadPool(int threadNum = 20);
        ThreadPool(ThreadPool &) = delete;
        ThreadPool(ThreadPool &&) = delete;
        ThreadPool &operator=(ThreadPool &) = delete;
        ThreadPool &operator=(ThreadPool &&) = delete;
        ~ThreadPool();

        using pTaskCallBack = void (*)(void *);
        void addTaskAndRun(pTaskCallBack cb, void *arg);

    private:
        void threadTask();

        std::atomic<bool> _stop;
        std::vector<std::thread> _threads;
        std::queue<pTaskCallBack> _cb;
        std::queue<void *> _arg;
        std::mutex _mutex;
        std::condition_variable _cond;
    };

} // namespace rtspsvr
