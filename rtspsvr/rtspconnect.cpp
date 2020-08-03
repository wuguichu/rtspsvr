#include "rtspconnect.h"
#include "rtspsvrinner.h"
#include "platformcommon.h"

namespace rtspsvr
{
    RtspConnect::RtspConnect(int sock, IoScheduler &scheduler)
        : m_sock(sock), m_scheduler(scheduler), m_cseq(0)
    {
        m_sendbuf[0] = '\0';
        m_scheduler.registerIoCallBack(m_sock, IoScheduler::SELECT_EXCEPTION | IoScheduler::SELECT_READ, handleRtspConnectSock, this);
    }

    RtspConnect::~RtspConnect()
    {
        closesocket(m_sock);
    }

    bool RtspConnect::handleRtspConnectSock(int selectType, void *arg)
    {
        RtspConnect *pthis = static_cast<RtspConnect *>(arg);
        return pthis->handleRtspConnectSock(selectType);
    }

    bool RtspConnect::handleRtspConnectSock(int selectType)
    {
        if (selectType & IoScheduler::SELECT_EXCEPTION)
        {
            RTSP_ERROR_S << "listen socket " << m_sock << " error,errno: " << getErrno() << "\n";
        }
        else if (selectType & IoScheduler::SELECT_READ)
        {
            struct sockaddr_in addr = {0};
            socklen_t len = sizeof(struct sockaddr_in);
            char buf[1024] = {0};
            int readlen = 0;
            readlen = recvfrom(m_sock, buf, sizeof(buf), 0, (struct sockaddr *)&addr, &len);
            if (readlen > 0)
            {
                RTSP_DEBUG_S << "recvfrom " << m_sock << " read len: " << readlen << "\n";
                RTSP_DEBUG_S << "buf:\n"
                             << buf << "\n";
                if (handleRtspRequest(buf) == true)
                    return true;
            }
            else if (readlen == 0)
                RTSP_INFO_S << "one connect is close, socket: " << m_sock << "\n";
            else if (readlen < 0)
                RTSP_ERROR_S << "listen socket " << m_sock << " read len: " << readlen << ""
                             << "\n";
        }
        return false;
    }

    bool RtspConnect::handleRtspRequest(char *buf)
    {
        char method[32] = {0};
        char *pCseq = nullptr;

        /*get the method*/
        if (buf == nullptr)
            return false;
        if (sscanf(buf, "%15s ", method) != 1)
            return false;
        if ((pCseq = strstr(buf, "CSeq: ")) == nullptr)
            return false;
        if (sscanf(pCseq, "CSeq: %u", &m_cseq) != 1)
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
        snprintf(m_sendbuf, sizeof(m_sendbuf) - 1,
                 "RTSP/1.0 200 OK\r\n"
                 "CSeq: %u\r\n"
                 "Public: OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY\r\n"
                 "\r\n",
                 m_cseq);
        return sendResponse();
    }

    bool RtspConnect::handleMethodDescribe(char *buf)
    {
        char url[256] = {0};
        if (sscanf(buf, "%*s %256s ", url) != 1)
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
                if (sscanf(p, "%*s %*[^=]=%u-%u", &rtpPort, &rtcpPort) != 2)
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
        if (send(m_sock, m_sendbuf, strlen(m_sendbuf) + 1, 0) <= 0)
            return false;
        return true;
    }
} // namespace rtspsvr