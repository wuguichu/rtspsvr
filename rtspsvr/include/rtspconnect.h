#pragma once

#include "logger.h"
#include "ioscheduler.h"

namespace rtspsvr
{
    const int rtspSendLen = 1024;

    class RtspConnect
    {
    public:
        RtspConnect(int sock, IoScheduler &scheduler);
        ~RtspConnect();

    private:
        static bool handleRtspConnectSock(int selectType, void *arg);
        bool handleRtspConnectSock(int selectType);
        bool handleRtspRequest(char *buf);

        bool handleMethodOptions();
        bool handleMethodDescribe(char *buf);
        bool handleMethodSetup(char *buf);
        bool sendResponse();

        int m_sock;
        IoScheduler &m_scheduler;
        unsigned m_cseq;
        char m_sendbuf[rtspSendLen];
    };
} // namespace rtspsvr