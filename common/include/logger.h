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
		static Logger *getInstance();
		void setSaveDir(const char *filedir = nullptr);
		std::string getPrefix(LogLevel logLevel, const char *func, int line);
		void writelog(const char *fmt, ...);
		Logger &operator<<(int i);
		Logger &operator<<(const char *str);
		Logger &operator<<(unsigned u);
		Logger &operator<<(double d);
		Logger &operator<<(void *p);

	private:
		Logger();
		Logger(const Logger &) = delete;
		Logger(const Logger &&) = delete;
		Logger &operator=(const Logger &) = delete;
		Logger &operator=(const Logger &&) = delete;
		~Logger();

		void writeThread();

		static Logger *m_pLogger;
		// static std::mutex m_InsMutex;
		char m_szFiledir[256];
		std::ofstream m_ofs;
		std::atomic<bool> m_bShutdown;
		std::thread m_thread;
		std::mutex m_mutex;
		std::condition_variable m_cond;
		std::queue<std::string> m_queue;
	};

#define LOG_LEVEL rtspsvr::Logger::LogDebug

#define LOG_DEBUG(fmt, ...)                                                                                                        \
	do                                                                                                                             \
	{                                                                                                                              \
		if (rtspsvr::Logger::LogDebug >= LOG_LEVEL)                                                                                \
		{                                                                                                                          \
			Logger::getInstance()->writelog("%s", Logger::getInstance()->getPrefix(Logger::LogDebug, __func__, __LINE__).c_str()); \
			Logger::getInstance()->writelog(fmt, ##__VA_ARGS__);                                                                   \
		}                                                                                                                          \
	} while (0)
#define LOG_INFO(fmt, ...)                                                                                                        \
	do                                                                                                                            \
	{                                                                                                                             \
		if (rtspsvr::Logger::LogInfo >= LOG_LEVEL)                                                                                \
		{                                                                                                                         \
			Logger::getInstance()->writelog("%s", Logger::getInstance()->getPrefix(Logger::LogInfo, __func__, __LINE__).c_str()); \
			Logger::getInstance()->writelog(fmt, ##__VA_ARGS__);                                                                  \
		}                                                                                                                         \
	} while (0)
#define LOG_WARN(fmt, ...)                                                                                                        \
	do                                                                                                                            \
	{                                                                                                                             \
		if (rtspsvr::Logger::LogWarn >= LOG_LEVEL)                                                                                \
		{                                                                                                                         \
			Logger::getInstance()->writelog("%s", Logger::getInstance()->getPrefix(Logger::LogWarn, __func__, __LINE__).c_str()); \
			Logger::getInstance()->writelog(fmt, ##__VA_ARGS__);                                                                  \
		}                                                                                                                         \
	} while (0)
#define LOG_ERROR(fmt, ...)                                                                                                        \
	do                                                                                                                             \
	{                                                                                                                              \
		if (rtspsvr::Logger::LogError >= LOG_LEVEL)                                                                                \
		{                                                                                                                          \
			Logger::getInstance()->writelog("%s", Logger::getInstance()->getPrefix(Logger::LogError, __func__, __LINE__).c_str()); \
			Logger::getInstance()->writelog(fmt, ##__VA_ARGS__);                                                                   \
		}                                                                                                                          \
	} while (0)

//Use stream format,Can't use std::endl yet
#define LOG_DEBUG_S                             \
	if (rtspsvr::Logger::LogDebug >= LOG_LEVEL) \
	*Logger::getInstance() << Logger::getInstance()->getPrefix(Logger::LogDebug, __func__, __LINE__).c_str()
#define LOG_INFO_S                             \
	if (rtspsvr::Logger::LogInfo >= LOG_LEVEL) \
	*Logger::getInstance() << Logger::getInstance()->getPrefix(Logger::LogInfo, __func__, __LINE__).c_str()
#define LOG_WARN_S                             \
	if (rtspsvr::Logger::LogWarn >= LOG_LEVEL) \
	*Logger::getInstance() << Logger::getInstance()->getPrefix(Logger::LogWarn, __func__, __LINE__).c_str()
#define LOG_ERROR_S                             \
	if (rtspsvr::Logger::LogError >= LOG_LEVEL) \
	*Logger::getInstance() << Logger::getInstance()->getPrefix(Logger::LogError, __func__, __LINE__).c_str()
} // namespace rtspsvr
