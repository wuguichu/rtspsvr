#include <chrono>
#include "ioscheduler.h"

namespace rtspsvr
{
IoScheduler::IoScheduler()
    : _maxfd(0)
{
    FD_ZERO(&_readSet);
    FD_ZERO(&_writeSet);
    FD_ZERO(&_execptionSet);
}
IoScheduler::~IoScheduler()
{
}

void IoScheduler::registerIoCallBack(int sock, int selectType, pIoEventCallBack cb, void *arg)
{
    if (sock < 0)
        return;

    FD_CLR(sock, &_readSet);
    FD_CLR(sock, &_writeSet);
    FD_CLR(sock, &_execptionSet);
    if (selectType & SELECT_READ)
        FD_SET(sock, &_readSet);
    if (selectType & SELECT_WRITE)
        FD_SET(sock, &_writeSet);
    if (selectType & SELECT_EXCEPTION)
        FD_SET(sock, &_execptionSet);

    IoCallBackData iocb = {this, sock, selectType, 0, cb, arg};
    auto it = _cb.find(sock);
    if (selectType == 0)
    {
        if (it != _cb.end())
            _cb.erase(it);
    }
    else
    {
        _cb[sock].cb = cb;
        _cb[sock].arg = arg;
    }

    _maxfd = _cb.empty() ? 0 : _cb.rbegin()->first + 1; //map will be sorted automatically
}

void IoScheduler::run()
{
    fd_set readSet = _readSet;
    fd_set writeSet = _writeSet;
    fd_set execptionSet = _execptionSet;
    int maxfd = _maxfd;
    struct timeval timeout = {0};
    int ret = 0;
    int selectType = 0;

    while (true)
    {
        readSet = _readSet;
        writeSet = _writeSet;
        execptionSet = _execptionSet;
        maxfd = _maxfd;
        timeout.tv_sec = 0;
        timeout.tv_usec = 10 * 1000;

        if (maxfd == 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        ret = select(maxfd, &readSet, &writeSet, &execptionSet, &timeout);
        if (ret < 0)
        {
            return;
        }
        if (ret == 0)
            continue;
        for (auto it = _cb.begin(); it != _cb.end(); it++)
        {
            selectType = 0;
            if (FD_ISSET(it->first, &readSet))
                selectType |= SELECT_READ;
            if (FD_ISSET(it->first, &writeSet))
                selectType |= SELECT_WRITE;
            if (FD_ISSET(it->first, &execptionSet))
                selectType |= SELECT_EXCEPTION;

            if (selectType != 0)
            {
                IoCallBackData *arg = new IoCallBackData(it->second);
                _threadPool.addTaskAndRun(runIoCallBackPool, arg);
                registerIoCallBack(it->first, 0, nullptr, nullptr);
            }
        }
    }
}

void IoScheduler::runIoCallBackPool(void *arg)
{
    IoCallBackData *p = static_cast<IoCallBackData *>(arg);
    p->pthis->runIoCallBack(p);
}

void IoScheduler::runIoCallBack(IoCallBackData *data)
{
    data->cb(data->acceptSelectType, data->arg);
    registerIoCallBack(data->sock, data->rowSelectType, data->cb, data->arg);
    delete (data);
}
} // namespace rtspsvr