#include <chrono>
#include <iostream>
#include <thread>

#include <stdlib.h>

#include "logger.h"

using namespace rtspsvr;

#undef LOG_LEVEL
#define LOG_LEVEL rtspsvr::Logger::LogInfo

int main()
{
	int i = 0;
	double d = 0.1;
	Logger::getInstance()->setSaveDir("./log");
	int *p = &i;

	while (true)
	{
		LOG_DEBUG("debug hello world!\n");
		LOG_DEBUG("debug %d\n", i);

		LOG_WARN("wran hello world!\n");
		LOG_WARN("wran %d\n", i);

		LOG_DEBUG_S << "DEBUG hello world"
					<< " " << i << " " << d + i << "\n";
		LOG_ERROR_S << "ERROR hello world"
					<< " " << i << " " << d + i << "\n";
		LOG_ERROR_S << "&i: " << p << "\n";

		if (i++ > 15)
			break;

		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
	return 0;
}