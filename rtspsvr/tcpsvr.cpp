#include "tcpsvr.h"
#include "rtspsvrinner.h"
#include "platformcommon.h"

namespace rtspsvr
{
    const int sock_linsten_count = 20;

    TcpSvr::TcpSvr(int port, IoScheduler &scheduler)
        : m_scheduler(scheduler)
    {
        if (initializeSock() != true)
        {
            RTSP_ERROR_S << "initializeSock error,errno: " << getErrno() << "\n";
            return;
        }
        m_sock = socket(AF_INET, SOCK_STREAM, 0);
        if (m_sock < 0)
        {
            RTSP_ERROR_S << "unable to create stream socket,errno: " << getErrno() << "\n";
            return;
        }
        int optval = 1;
        setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&optval, sizeof(optval));
        struct sockaddr_in addr = {0};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = INADDR_ANY;
        if (bind(m_sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        {
            closesocket(m_sock);
            RTSP_ERROR_S << "bind socket error,errno: " << getErrno() << "\n";
            return;
        }
        listen(m_sock, sock_linsten_count);
        RTSP_DEBUG_S << "success to create stream socket: " << m_sock << "\n";

        m_scheduler.registerIoCallBack(m_sock, IoScheduler::SELECT_READ | IoScheduler::SELECT_EXCEPTION, listenCallBack, this);
    }

    TcpSvr::~TcpSvr()
    {
        if (m_sock)
            closesocket(m_sock);
    }

    bool TcpSvr::listenCallBack(int selectType, void *arg)
    {
        TcpSvr *p = static_cast<TcpSvr *>(arg);
        return p->listenCallBack(selectType);
    }

    bool TcpSvr::listenCallBack(int selectType)
    {
        if (selectType & IoScheduler::SELECT_EXCEPTION)
        {
            RTSP_ERROR_S << "listen socket " << m_sock << " error,errno: " << getErrno() << "\n";
            closesocket(m_sock);
            return false;
        }
        else if (selectType & IoScheduler::SELECT_READ)
        {
            struct sockaddr_in addr = {0};
            socklen_t addrlen = sizeof(struct sockaddr_in);
            int connfd = accept(m_sock, (struct sockaddr *)&addr, &addrlen);
            if (connfd < 0)
            {
                RTSP_ERROR_S << "accept socket error,errno: " << getErrno() << "\n";
                return true;
            }
            RTSP_INFO_S << "accept a new socket: " << connfd << "\n";
            handNewConnect(connfd);
            return true;
        }
        return false;
    }
} // namespace rtspsvr