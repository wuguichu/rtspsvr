#include "rtspconnect.h"
#include "rtspsvrinner.h"
#include "platformcommon.h"

namespace rtspsvr
{
RtspConnect::RtspConnect(int sock, Logger &log, IoScheduler &scheduler)
    : _sock(sock), _log(log), _scheduler(scheduler), _cseq(0)
{
    _sendbuf[0] = '\0';
    _scheduler.registerIoCallBack(_sock, IoScheduler::SELECT_EXCEPTION | IoScheduler::SELECT_READ, handRtspConnectSock, this);
}

RtspConnect::~RtspConnect()
{
    closesocket(_sock);
}

bool RtspConnect::handRtspConnectSock(int selectType, void *arg)
{
    RtspConnect *pthis = static_cast<RtspConnect *>(arg);
    return pthis->handRtspConnectSock(selectType);
}

bool RtspConnect::handRtspConnectSock(int selectType)
{
    if (selectType & IoScheduler::SELECT_EXCEPTION)
    {
        RTSP_ERROR_S << "listen socket " << _sock << " error,errno: " << getErrno() << "\n";
    }
    else if (selectType & IoScheduler::SELECT_READ)
    {
        struct sockaddr_in addr = {0};
        socklen_t len = sizeof(struct sockaddr_in);
        char buf[1024] = {0};
        int readlen = 0;
        readlen = recvfrom(_sock, buf, sizeof(buf), 0, (struct sockaddr *)&addr, &len);
        if (readlen > 0)
        {
            RTSP_DEBUG_S << "recvfrom " << _sock << " read len: " << readlen << "\n";
            RTSP_DEBUG_S << "buf:\n"
                         << buf << "\n";
            if (handRtspRequest(buf) == true)
                return true;
        }
        //else if (readlen == 0)
        //RTSP_DEBUG_S << "one connect is close, socket: " << _sock << "\n";
        //else if (readlen < 0)
        //RTSP_ERROR_S << "listen socket " << _sock << " read len: " << readlen << "\n";
    }
    return false;
}

bool RtspConnect::handRtspRequest(char *buf)
{
    char method[32] = {0};
    char *pCseq = nullptr;

    /*get the method*/
    if (buf == nullptr)
        return false;
    if (sscanf_s(buf, "%15s ", method, sizeof(method) - 1) != 1)
        return false;
    if ((pCseq = strstr(buf, "CSeq: ")) == nullptr)
        return false;
    if (sscanf_s(pCseq, "CSeq: %u", &_cseq) != 1)
        return false;

    if (!strcmp(method, "OPTIONS"))
        return handleMethodOptions();
    else if (!strcmp(method, "DESCRIBE"))
        return handleMethodDescribe(buf);
    else if (!strcmp(method, "SETUP"))
        return handleMethodSetup(buf);

    return true;
}

bool RtspConnect::handleMethodOptions()
{
    snprintf(_sendbuf, sizeof(_sendbuf) - 1,
             "RTSP/1.0 200 OK\r\n"
             "CSeq: %u\r\n"
             "Public: OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY\r\n"
             "\r\n",
             _cseq);
    return sendResponse();
}

bool RtspConnect::handleMethodDescribe(char *buf)
{
    char url[256] = {0};
    if (sscanf_s(buf, "%*s %256s ", url, sizeof(url) - 1) != 1)
        return false;
    char *pMedia = nullptr;
    if ((pMedia = strrchr(url, '/')) == nullptr)
        return false;
    RTSP_INFO_S << pMedia + 1 << "\n";

    return true;
}

bool RtspConnect::handleMethodSetup(char *buf)
{
    char *p = nullptr;
    if ((p = strstr(buf, "Transport: ")) == nullptr)
        return false;
    RTSP_INFO_S << "\n";
    if (strstr(buf, "RTP/AVP") != nullptr)
    {
        RTSP_INFO_S << "\n";
        if (strstr(p, "unicast"))
        {
            int rtpPort = 0, rtcpPort = 0;
            if (sscanf_s(p, "%*s %*[^=]=%u-%u", &rtpPort, &rtcpPort) != 2)
                return false;
            RTSP_INFO_S << rtpPort << "  " << rtcpPort << "\n";
        }
        else if (strstr(p, "multicast"))
        {
        }
        else
            return false;
    }
    else if (strstr(buf, "RTP/AVP/TCP") != nullptr)
    {
    }
    
    return true;
}

bool RtspConnect::sendResponse()
{
    if (send(_sock, _sendbuf, strlen(_sendbuf) + 1, 0) <= 0)
        return false;
    return true;
}
} // namespace rtspsvr