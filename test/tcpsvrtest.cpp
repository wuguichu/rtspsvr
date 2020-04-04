#include <iostream>
#include<chrono>
#include<thread>
#include "tcpsvr.h"

int main()
{
	printhello();
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    return 0;
}