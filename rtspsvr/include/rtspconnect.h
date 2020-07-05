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
    static bool handRtspConnectSock(int selectType, void *arg);
    bool handRtspConnectSock(int selectType);
    bool handRtspRequest(char *buf);

    bool handleMethodOptions();
    bool handleMethodDescribe(char *buf);
    bool handleMethodSetup(char *buf);
    bool sendResponse();

    int _sock;
    IoScheduler &_scheduler;
    unsigned _cseq;
    char _sendbuf[rtspSendLen];
};
} // namespace rtspsvr