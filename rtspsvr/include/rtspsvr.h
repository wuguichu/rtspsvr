#pragma once

#include "tcpsvr.h"
#include "logger.h"
#include "ioscheduler.h"

namespace rtspsvr
{
    class RtspSvr : public TcpSvr
    {
    public:
        RtspSvr(int port, IoScheduler &scheduler);
        ~RtspSvr();

    protected:
        virtual void handNewConnect(int sockfd);

    private:
        IoScheduler &_scheduler;
    };

} // namespace rtspsvr