#include <iostream>
#include <chrono>
#include <thread>
#include "tcpsvr.h"
#include "ioscheduler.h"
#include "logger.h"

using namespace rtspsvr;

class TestTcpSvr : public TcpSvr
{
public:
    TestTcpSvr(int port, IoScheduler &scheduler) : TcpSvr(port, scheduler) {}

protected:
    virtual void handNewConnect(int sockfd)
    {
        LOG_INFO_S << "connected, sock: " << sockfd << "\n";
    }
};

int main()
{
    LOG_DEBUG_S << "test tcp server!\n";

    IoScheduler ioScheduler;
    TestTcpSvr testTcpSvr(6767, ioScheduler);
    ioScheduler.run();

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    return 0;
}