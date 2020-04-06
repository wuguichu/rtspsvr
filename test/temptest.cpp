#include <chrono>
#include <iostream>
#include <thread>

#include <stdlib.h>

#include "logger.h"

using namespace rtspsvr;

int main()
{
	int i = 0;
	double d = 0.1;
	Logger log(Logger::LogInfo, "./log");
	int *p = &i;

	while (true)
	{
		LOG_DEBUG(log, "debug hello world!\n");
		LOG_DEBUG(log, "debug %d\n", i);

		LOG_WARN(log, "wran hello world!\n");
		LOG_WARN(log, "wran %d\n", i);

		LOG_DEBUG_S(log) << "DEBUG hello world"
						 << " " << i << " " << d + i << "\n";
		LOG_ERROR_S(log) << "ERROR hello world"
						 << " " << i << " " << d + i << "\n";
		LOG_ERROR_S(log) << "&i: " << p << "\n";

		if (i++ > 1000)
			break;

		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}
	return 0;
}