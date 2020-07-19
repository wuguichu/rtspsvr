#pragma once

#include <unordered_map>
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
        IoScheduler &m_scheduler;
        std::unordered_map<int, RtspConnect *> m_map;
    };

} // namespace rtspsvr