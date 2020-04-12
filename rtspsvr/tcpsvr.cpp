#include "tcpsvr.h"
#include "rtspsvrinner.h"
#include "platformcommon.h"

namespace rtspsvr
{
const int sock_linsten_count = 20;

TcpSvr::TcpSvr(int port, Logger &log, IoScheduler &scheduler)
    : _log(log), _scheduler(scheduler)
{
    if (initializeSock() != true)
    {
        RTSP_ERROR_S << "initializeSock error,errno: " << getErrno() << "\n";
        return;
    }
    _sock = socket(AF_INET, SOCK_STREAM, 0);
    if (_sock < 0)
    {
        RTSP_ERROR_S << "unable to create stream socket,errno: " << getErrno() << "\n";
        return;
    }
    int optval = 1;
    setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&optval, sizeof(optval));
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(_sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        closesocket(_sock);
        RTSP_ERROR_S << "bind socket error,errno: " << getErrno() << "\n";
        return;
    }
    listen(_sock, sock_linsten_count);
    RTSP_DEBUG_S << "success to create stream socket: " << _sock << "\n";

    _scheduler.registerIoCallBack(_sock, IoScheduler::SELECT_READ | IoScheduler::SELECT_EXCEPTION, listenCallBack, this);
}

TcpSvr::~TcpSvr()
{
    if (_sock)
        closesocket(_sock);
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
        closesocket(_sock);
        RTSP_ERROR_S << "listen socket " << _sock << " error,errno: " << getErrno() << "\n";
        return false;
    }
    else if (selectType & IoScheduler::SELECT_READ)
    {
        struct sockaddr_in addr = {0};
        socklen_t addrlen = sizeof(struct sockaddr_in);
        int connfd = accept(_sock, (struct sockaddr *)&addr, &addrlen);
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