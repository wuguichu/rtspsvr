#pragma once

#include "logger.h"
#include "ioscheduler.h"

namespace rtspsvr
{
    class TcpSvr
    {
    public:
        TcpSvr(int port, IoScheduler &scheduler);
        virtual ~TcpSvr();

    protected:
        virtual void handNewConnect(int sockfd) {}

    private:
        static bool listenCallBack(int selectType, void *arg);
        bool listenCallBack(int selectType);

        IoScheduler &m_scheduler;
        int m_sock;
    };

} // namespace rtspsvr