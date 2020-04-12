#include <chrono>
#include <iostream>
#include <thread>

#include <stdlib.h>

#include "logger.h"
#include "threadpool.h"
#include "ioscheduler.h"
#include "tcpsvr.h"

using namespace rtspsvr;

Logger m_log(Logger::LogDebug);

/* 
rtsp://127.0.0.1:554/test.h264
rtsp://192.168.117.1/test.aac
<filename>
*/

int main()
{
	LOG_INFO_S(m_log) << "func running\n";

	IoScheduler ioScheduler;
	TcpSvr tcpsvr(554, m_log, ioScheduler);
	ioScheduler.run();

	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	return 0;
}