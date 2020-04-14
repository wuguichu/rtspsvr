#pragma once

#include "tcpsvr.h"
#include "logger.h"
#include "ioscheduler.h"

namespace rtspsvr
{
class RtspSvr : public TcpSvr
{
public:
    RtspSvr(int port, Logger &log, IoScheduler &scheduler);
    ~RtspSvr();

protected:
    virtual void handNewConnect(int sockfd);

private:
    Logger &_log;
    IoScheduler &_scheduler;
};

} // namespace rtspsvr