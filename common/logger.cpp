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
	Logger *Logger::m_pLogger = nullptr;
	std::mutex Logger::m_InsMutex;

	const char LogLevelString[][16] = {
		"debug",
		"info",
		"warn",
		"error",
	};

	Logger *Logger::getInstance()
	{
		if (nullptr == m_pLogger)
		{
			std::unique_lock<std::mutex> lock(m_InsMutex);
			if (nullptr == m_pLogger)
			{
				m_pLogger = new Logger();
			}
		}
		return m_pLogger;
	}

	void Logger::setSaveDir(const char *filedir)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		if (m_ofs.is_open())
		{
			m_ofs.close();
		}
		if (filedir)
		{
			strncpy(m_szFiledir, filedir, sizeof(m_szFiledir) - 1);
#if defined(WIN32) || defined(_WIN32)
			CreateDirectory(m_szFiledir, NULL);
#else
			mkdir(m_szFiledir, S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
#endif
		}
		else
			m_szFiledir[0] = '\0';
	}

	Logger::Logger()
		: m_bShutdown(false)
	{
		m_szFiledir[0] = '\0';
		m_thread = std::thread(&Logger::writeThread, this);
	}

	Logger::~Logger()
	{
		m_bShutdown = true;
		m_cond.notify_all();
		m_thread.join();
		if (m_ofs.is_open())
		{
			m_ofs.close();
		}
	}

	std::string Logger::getPrefix(LogLevel logLevel, const char *func, int line)
	{
		time_t t = time(0);
		struct tm *newtime = localtime(&t);

		char ch[32] = {0};
		char buf[64] = {0};
		strftime(ch, sizeof(ch), "%Y-%m-%d %H-%M-%S", localtime(&t));
		snprintf(buf, sizeof(buf) - 1, "%s [%5s]: [%s][%d] ", ch,
				 LogLevelString[logLevel], func, line);

		return std::string(buf);
	}

	void Logger::writelog(const char *fmt, ...)
	{
		char buf[1024] = {0};

		va_list args;
		va_start(args, fmt);
		vsprintf(buf + strlen(buf), fmt, args);
		va_end(args);

		std::string entry(buf);
		std::unique_lock<std::mutex> lock(m_mutex);
		m_queue.push(std::move(entry));
		m_cond.notify_all();
	}

	void Logger::writeThread()
	{
		time_t t = time(0);
		struct tm *newtime = nullptr;
		char ch[64] = {0};
		char pathname[256 + 64] = {0};
		std::unique_lock<std::mutex> lock(m_mutex);

		while (true)
		{
			if (m_bShutdown && m_queue.empty())
			{
				m_ofs.flush();
				break;
			}

			if (!m_queue.empty())
			{
				if (strlen(m_szFiledir) != 0)
				{
					t = time(0);
					newtime = localtime(&t);
					strftime(ch, sizeof(ch), "%Y-%m-%d-%H-%M-%S", localtime(&t));
					if (!m_ofs.is_open() || (newtime->tm_hour % 12 == 0 &&
											 newtime->tm_min == 0 && newtime->tm_sec == 0))
					{
						if (m_ofs.is_open())
						{
							m_ofs.close();
						}
						snprintf(pathname, sizeof(pathname), "%s/%s.log", m_szFiledir, ch);
						m_ofs.open(pathname);
						if (m_ofs.fail())
						{
							std::cerr << "Logger Failed to open logfile." << std::endl;
						}
					}
				}

				while (!m_queue.empty())
				{
					if (m_ofs.is_open())
					{
						m_ofs << m_queue.front();
					}
					else
						std::cout << m_queue.front();
					m_queue.pop();
				}
				if (m_ofs.is_open())
					m_ofs.flush();
			}
			else
			{
				m_cond.wait(lock);
			}
		}
	}

	Logger &Logger::operator<<(int i)
	{
		std::string entry(std::to_string(i));
		std::unique_lock<std::mutex> lock(m_mutex);
		m_queue.push(std::move(entry));
		m_cond.notify_all();
		return *this;
	}

	Logger &Logger::operator<<(const char *str)
	{
		std::string entry(str);
		std::unique_lock<std::mutex> lock(m_mutex);
		m_queue.push(std::move(entry));
		m_cond.notify_all();
		return *this;
	}

	Logger &Logger::operator<<(unsigned u)
	{
		std::string entry(std::to_string(u));
		std::unique_lock<std::mutex> lock(m_mutex);
		m_queue.push(std::move(entry));
		m_cond.notify_all();
		return *this;
	}

	Logger &Logger::operator<<(double d)
	{
		std::string entry(std::to_string(d));
		std::unique_lock<std::mutex> lock(m_mutex);
		m_queue.push(std::move(entry));
		m_cond.notify_all();
		return *this;
	}

	Logger &Logger::operator<<(void *p)
	{
		char buf[64] = {0};

		snprintf(buf, sizeof(buf) - 1, "%p", p);
		std::string entry(buf);
		std::unique_lock<std::mutex> lock(m_mutex);
		m_queue.push(std::move(entry));
		m_cond.notify_all();
		return *this;
	}
} // namespace rtspsvr
