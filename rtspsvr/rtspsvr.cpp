#include "rtspsvr.h"
#include "rtspconnect.h"
#include "rtspsvrinner.h"

namespace rtspsvr
{
    RtspSvr::RtspSvr(int port, IoScheduler &scheduler)
        : m_scheduler(scheduler), TcpSvr(port, scheduler)
    {
    }

    RtspSvr::~RtspSvr()
    {
    }

    void RtspSvr::handNewConnect(int sockfd)
    {
        RtspConnect *pCon = new RtspConnect(sockfd, m_scheduler);
        if (pCon == nullptr)
        {
            RTSP_ERROR_S << "new RtspConnect failed\n";
            closesocket(sockfd);
            return;
        }
        m_map.insert(std::make_pair(sockfd, pCon));
    }
} // namespace rtspsvr