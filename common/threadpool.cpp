#include "threadpool.h"

namespace rtspsvr
{

ThreadPool::ThreadPool(int threadNum)
    : _stop(false)
{
    for (size_t i = 0; i < threadNum; i++)
    {
        _threads.push_back(std::thread(&ThreadPool::threadTask, this));
    }
}

ThreadPool::~ThreadPool()
{
    _stop = true;
    _cond.notify_all();
    for (auto &th : _threads)
    {
        th.join();
    }
}

void ThreadPool::addTaskAndRun(pTaskCallBack cb, void *arg)
{
    if (!cb)
        return;

    std::unique_lock<std::mutex> lock(_mutex);
    _cb.push(cb);
    _arg.push(arg);
    _cond.notify_one();
}

void ThreadPool::threadTask()
{
    while (!_stop)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if (_cb.empty())
        {
            _cond.wait(lock);
            continue;
        }
        if (_stop)
            return;
        pTaskCallBack cb = _cb.front();
        _cb.pop();
        void *arg = _arg.front();
        _arg.pop();
        cb(arg);
    }
}

} // namespace rtspsvr