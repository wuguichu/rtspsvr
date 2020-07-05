#include "rtspsvr.h"
#include "rtspconnect.h"
#include "rtspsvrinner.h"

namespace rtspsvr
{
    RtspSvr::RtspSvr(int port, IoScheduler &scheduler)
        : _scheduler(scheduler), TcpSvr(port, scheduler)
    {
    }

    RtspSvr::~RtspSvr()
    {
    }

    void RtspSvr::handNewConnect(int sockfd)
    {
        RtspConnect *pCon = new RtspConnect(sockfd, _scheduler);
        if (pCon == nullptr)
        {
            RTSP_ERROR_S << "new RtspConnect failed\n";
        }
    }
} // namespace rtspsvr