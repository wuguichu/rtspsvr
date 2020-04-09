#include <chrono>
#include <iostream>
#include <thread>

#include <stdlib.h>

#include "logger.h"
#include "threadpool.h"

using namespace rtspsvr;

Logger m_log(Logger::LogInfo);

void func(void *arg)
{
	char *p = static_cast<char *>(arg);
	LOG_INFO_S(m_log) << "func running " << p << "\n";
}

int main()
{
	LOG_ERROR_S(m_log) << "func running\n";
	LOG_ERROR(m_log, "func running\n");

	ThreadPool p;
	int i = 0;
	char buf[32] = {"hello world"};

	while (1)
	{
		i++;
		//调整线程之间cpu调度,可以去掉
		//this_thread::sleep_for(chrono::seconds(1));
		//auto task = bind(func);
		//std::function<void()> f = func;
		//auto task = std::bind(func, static_cast<void *>(&i));
		p.addTaskAndRun(func, buf);
		if (i > 15)
			break;
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	return 0;
}