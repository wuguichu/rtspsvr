#include<iostream>
#include <cstdarg>
#include <ctime>
#include <cstring>

#include "logger.h"

namespace rtspsvr
{
	const char LogLevelString[][16] = {
		"debug",
		"info",
		"warn",
		"error",
	};

	Logger::Logger(LogLevel logLevel, const char* filedir)
		:_level(logLevel), _shutdown(false)
	{
		if (filedir)
			strncpy(_filedir, filedir, sizeof(_filedir) - 1);
		else
			_filedir[0] = '\0';

		_thread = std::thread(&Logger::writeThread, this);
	}

	Logger::~Logger()
	{
		_shutdown = true;
		_cond.notify_all();
		_thread.join();
		if (_ofs.is_open()) {
			_ofs.close();
		}
	}

	void Logger::writelog(LogLevel logLevel, const char* func, int line, const char* fmt, ...)
	{
		if (logLevel < _level)
			return;

		char buf[2048] = { 0 };

		snprintf(buf, sizeof(buf), " [%5s] : [%s][%d] ", LogLevelString[logLevel], func, line);
		va_list args;
		va_start(args, fmt);
		vsprintf(buf + strlen(buf), fmt, args);
		va_end(args);

		std::string entry(buf);
		std::unique_lock<std::mutex> lock(_mutex);
		_queue.push(std::move(entry));
		_cond.notify_all();
	}

	void Logger::writeThread()
	{
		time_t t = time(0);
		struct tm* newtime = nullptr;
		char ch[64] = { 0 };
		char pathname[256] = { 0 };
		std::unique_lock<std::mutex> lock(_mutex);

		while (!_shutdown) {
			if (!_queue.empty()) {
				t = time(0);
				newtime = localtime(&t);
				strftime(ch, sizeof(ch), "%Y-%m-%d %H-%M-%S", localtime(&t));

				if (strlen(_filedir) != 0) {
					if (!_ofs.is_open() || (newtime->tm_hour % 12 == 0 && newtime->tm_min == 0 && newtime->tm_sec == 0)) {
						if (_ofs.is_open()) {
							_ofs.close();
						}
						snprintf(pathname, sizeof(pathname), "%s/%s.log", _filedir, ch);
						_ofs.open(pathname);
						if (_ofs.fail()) {
							std::cerr << "Logger Failed to open logfile." << std::endl;
						}
					}
				}

				if (_ofs.is_open()) {
					_ofs << ch << _queue.front() << std::endl;
				}
				else
					std::cout << ch << _queue.front() << std::endl;
				_queue.pop();
			}
			else {
				_cond.wait(lock);
			}
		}
	}
}
