#include <chrono>
#include <iostream>
#include <thread>

#include <stdlib.h>

#include "logger.h"
#include "threadpool.h"
#include "ioscheduler.h"

using namespace rtspsvr;

Logger m_log(Logger::LogInfo);

void stdinSelect(int selectType, void *arg)
{
	LOG_ERROR_S(m_log) << "access run\n";
}

int main()
{
	LOG_ERROR_S(m_log) << "func running\n";

	IoScheduler scheduler;

	scheduler.registerIoCallBack(0, IoScheduler::SELECT_READ, stdinSelect, nullptr);
	scheduler.run();

	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	return 0;
}