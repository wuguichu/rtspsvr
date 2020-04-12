#pragma once

#include <map>
#include "threadpool.h"
#if defined(_WIN32) || defined(WIN32)
#include <WinSock2.h>
#else
#endif

namespace rtspsvr
{
class IoScheduler
{
public:
    enum SelectType
    {
        SELECT_READ = 1 << 0,
        SELECT_WRITE = 1 << 1,
        SELECT_EXCEPTION = 1 << 2,
    };

public:
    IoScheduler();
    ~IoScheduler();

    //selectType is the OR operator combination of SelectType,If it is equal to 0, it will remove the monitoring socket
    //If it returns true, continue to listen
    using pIoEventCallBack = bool (*)(int selectType, void *arg);
    void registerIoCallBack(int sock, int selectType, pIoEventCallBack cb, void *arg);
    void run(); //This will block the thread

private:
    struct IoCallBackData
    {
        IoScheduler *pthis;
        int sock;
        int rowSelectType;
        int acceptSelectType;
        pIoEventCallBack cb;
        void *arg;
    };
    using IoDataMap = std::map<int, IoCallBackData>;
    static void runIoCallBackPool(void *arg);
    void runIoCallBack(IoCallBackData *data);

    fd_set _readSet;
    fd_set _writeSet;
    fd_set _execptionSet;
    int _maxfd;
    IoDataMap _cb;
    ThreadPool _threadPool;
};
} // namespace rtspsvr