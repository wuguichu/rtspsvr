#pragma once

#include "logger.h"
#include "ioscheduler.h"

namespace rtspsvr
{
class TcpSvr
{
public:
    TcpSvr(int port, Logger &log, IoScheduler &scheduler);
    virtual ~TcpSvr();

    virtual void handNewConnect(int sockfd) {}

private:
    static bool listenCallBack(int selectType, void *arg);
    bool listenCallBack(int selectType);

    Logger &_log;
    IoScheduler &_scheduler;
    int _sock;
};

} // namespace rtspsvr