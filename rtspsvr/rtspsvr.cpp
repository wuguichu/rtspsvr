#include "rtspsvr.h"
#include "rtspconnect.h"
#include "rtspsvrinner.h"

namespace rtspsvr
{
RtspSvr::RtspSvr(int port, Logger &log, IoScheduler &scheduler)
    : _log(log), _scheduler(scheduler), TcpSvr(port, log, scheduler)
{
}

RtspSvr::~RtspSvr()
{
}

void RtspSvr::handNewConnect(int sockfd)
{
    RtspConnect *pCon = new RtspConnect(sockfd, _log, _scheduler);
    if (pCon == nullptr)
    {
        RTSP_ERROR_S << "new RtspConnect failed\n";
    }
}
} // namespace rtspsvr