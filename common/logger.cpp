#include <cstdarg>
#include <cstring>
#include <ctime>
#include <iostream>

#include "logger.h"

#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

namespace rtspsvr
{
const char LogLevelString[][16] = {
	"debug",
	"info",
	"warn",
	"error",
};

Logger::Logger(LogLevel logLevel, const char *filedir)
	: _level(logLevel), _shutdown(false)
{
	if (filedir)
	{
		strncpy(_filedir, filedir, sizeof(_filedir) - 1);
#if defined(WIN32) || defined(_WIN32)
		CreateDirectory(_filedir, NULL);
#else
		mkdir(_filedir, S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
#endif
	}
	else
		_filedir[0] = '\0';

	_thread = std::thread(&Logger::writeThread, this);
}

Logger::~Logger()
{
	_shutdown = true;
	_cond.notify_all();
	_thread.join();
	if (_ofs.is_open())
	{
		_ofs.close();
	}
}

std::string Logger::getPrefix(LogLevel logLevel, const char *func, int line)
{
	if (logLevel < _level)
		return std::string();

	time_t t = time(0);
	struct tm *newtime = localtime(&t);

	char ch[32] = {0};
	char buf[64] = {0};
	strftime(ch, sizeof(ch), "%Y-%m-%d %H-%M-%S", localtime(&t));
	snprintf(buf, sizeof(buf) - 1, "%s [%5s]: [%s][%d] ", ch,
			 LogLevelString[logLevel], func, line);

	return std::string(buf);
}

void Logger::writelog(LogLevel logLevel, const char *fmt, ...)
{
	if (logLevel < _level)
		return;

	char buf[1024] = {0};

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
	struct tm *newtime = nullptr;
	char ch[64] = {0};
	char pathname[256 + 64] = {0};
	std::unique_lock<std::mutex> lock(_mutex);

	while (!_shutdown)
	{
		if (!_queue.empty())
		{
			if (strlen(_filedir) != 0)
			{
				t = time(0);
				newtime = localtime(&t);
				strftime(ch, sizeof(ch), "%Y-%m-%d-%H-%M-%S", localtime(&t));
				if (!_ofs.is_open() || (newtime->tm_hour % 12 == 0 &&
										newtime->tm_min == 0 && newtime->tm_sec == 0))
				{
					if (_ofs.is_open())
					{
						_ofs.close();
					}
					snprintf(pathname, sizeof(pathname), "%s/%s.log", _filedir, ch);
					_ofs.open(pathname);
					if (_ofs.fail())
					{
						std::cerr << "Logger Failed to open logfile." << std::endl;
					}
				}
			}

			if (_ofs.is_open())
			{
				_ofs << _queue.front();
			}
			else
				std::cout << _queue.front();
			_queue.pop();
		}
		else
		{
			_cond.wait(lock);
		}
	}
}

Logger &Logger::operator<<(int i)
{
	std::string entry(std::to_string(i));
	std::unique_lock<std::mutex> lock(_mutex);
	_queue.push(std::move(entry));
	_cond.notify_all();
	return *this;
}

Logger &Logger::operator<<(const char *str)
{
	std::string entry(str);
	std::unique_lock<std::mutex> lock(_mutex);
	_queue.push(std::move(entry));
	_cond.notify_all();
	return *this;
}

Logger &Logger::operator<<(unsigned u)
{
	std::string entry(std::to_string(u));
	std::unique_lock<std::mutex> lock(_mutex);
	_queue.push(std::move(entry));
	_cond.notify_all();
	return *this;
}

Logger &Logger::operator<<(double d)
{
	std::string entry(std::to_string(d));
	std::unique_lock<std::mutex> lock(_mutex);
	_queue.push(std::move(entry));
	_cond.notify_all();
	return *this;
}

Logger &Logger::operator<<(void *p)
{
	char buf[64] = {0};

	snprintf(buf, sizeof(buf) - 1, "%p", p);
	std::string entry(buf);
	std::unique_lock<std::mutex> lock(_mutex);
	_queue.push(std::move(entry));
	_cond.notify_all();
	return *this;
}
} // namespace rtspsvr
