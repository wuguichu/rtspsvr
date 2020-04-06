#pragma once

#include <thread>
#include <fstream>
#include <atomic>
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>

namespace rtspsvr
{
class Logger
{
public:
	enum LogLevel
	{
		LogDebug,
		LogInfo,
		LogWarn,
		LogError,
		LogClose,
	};

public:
	/*If filedir is null, then output to std::cout*/
	Logger(LogLevel logLevel, const char *filedir = nullptr);
	Logger(const Logger &) = delete;
	Logger(const Logger &&) = delete;
	Logger &operator=(const Logger &) = delete;
	Logger &operator=(const Logger &&) = delete;
	~Logger();

	LogLevel getLevel() { return _level; }
	std::string getPrefix(LogLevel logLevel, const char *func, int line);
	void writelog(LogLevel logLevel, const char *fmt, ...);
	Logger &operator<<(int i);
	Logger &operator<<(const char *str);
	Logger &operator<<(unsigned u);
	Logger &operator<<(double d);
	Logger &operator<<(void *p);

private:
	void writeThread();

	LogLevel _level;
	char _filedir[256];
	std::ofstream _ofs;
	std::atomic<bool> _shutdown;
	std::thread _thread;
	std::mutex _mutex;
	std::condition_variable _cond;
	std::queue<std::string> _queue;
};

#define LOG_DEBUG(log, fmt, ...)                                                                                    \
	do                                                                                                              \
	{                                                                                                               \
		log.writelog(rtspsvr::Logger::LogDebug, "%s", log.getPrefix(Logger::LogDebug, __func__, __LINE__).c_str()); \
		log.writelog(rtspsvr::Logger::LogDebug, fmt, ##__VA_ARGS__);                                                \
	} while (0)
#define LOG_INFO(log, fmt, ...)                                                                                   \
	do                                                                                                            \
	{                                                                                                             \
		log.writelog(rtspsvr::Logger::LogInfo, "%s", log.getPrefix(Logger::LogInfo, __func__, __LINE__).c_str()); \
		log.writelog(rtspsvr::Logger::LogInfo, fmt, ##__VA_ARGS__);                                               \
	} while (0)
#define LOG_WARN(log, fmt, ...)                                                                                   \
	do                                                                                                            \
	{                                                                                                             \
		log.writelog(rtspsvr::Logger::LogWarn, "%s", log.getPrefix(Logger::LogWarn, __func__, __LINE__).c_str()); \
		log.writelog(rtspsvr::Logger::LogWarn, fmt, ##__VA_ARGS__);                                               \
	} while (0)
#define LOG_ERROR(log, fmt, ...)                                                                                    \
	do                                                                                                              \
	{                                                                                                               \
		log.writelog(rtspsvr::Logger::LogError, "%s", log.getPrefix(Logger::LogError, __func__, __LINE__).c_str()); \
		log.writelog(rtspsvr::Logger::LogError, fmt, ##__VA_ARGS__);                                                \
	} while (0)

//Use stream format,Can't use std::endl yet
#define LOG_DEBUG_S(log)                             \
	if (rtspsvr::Logger::LogDebug >= log.getLevel()) \
	log << log.getPrefix(Logger::LogDebug, __func__, __LINE__).c_str()
#define LOG_INFO_S(log)                             \
	if (rtspsvr::Logger::LogInfo >= log.getLevel()) \
	log << log.getPrefix(Logger::LogInfo, __func__, __LINE__).c_str()
#define LOG_WARN_S(log)                             \
	if (rtspsvr::Logger::LogWarn >= log.getLevel()) \
	log << log.getPrefix(Logger::LogWarn, __func__, __LINE__).c_str()
#define LOG_ERROR_S(log)                             \
	if (rtspsvr::Logger::LogError >= log.getLevel()) \
	log << log.getPrefix(Logger::LogError, __func__, __LINE__).c_str()
} // namespace rtspsvr
