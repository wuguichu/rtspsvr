#include <iostream>
#include <stdlib.h>
#include<chrono>
#include<thread>

#include "logger.h"

using namespace rtspsvr;

int main()
{
	int i = 0;
	Logger log(Logger::LogDebug, ".");

	while (true){
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));

		LOG_DEBUG(log, " debug hello world!");
		LOG_DEBUG(log, " debug %d", i);

		LOG_WARN(log, " wran hello world!");
		LOG_WARN(log, " wran %d", i);

		if (i++ > 1000)
			break;
	}
    return 0;
}