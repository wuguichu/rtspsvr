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
		};

	public:
		/*If filedir is null, then output to std::cout*/
		Logger(LogLevel logLevel, const char* filedir = nullptr);
		Logger(const Logger&) = delete;
		Logger(const Logger&&) = delete;
		Logger& operator=(const Logger&) = delete;
		Logger& operator=(const Logger&&) = delete;
		~Logger();

		void writelog(LogLevel logLevel, const char* func, int line, const char* fmt, ...);
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

	#define LOG_DEBUG(log, fmt, ...)		log.writelog(rtspsvr::Logger::LogDebug, __func__, __LINE__, fmt, ##__VA_ARGS__)
	#define LOG_INFO(log, fmt, ...)		log.writelog(rtspsvr::Logger::LogInfo, __func__, __LINE__, fmt, ##__VA_ARGS__)
	#define LOG_WARN(log, fmt, ...)		log.writelog(rtspsvr::Logger::LogWarn, __func__, __LINE__, fmt, ##__VA_ARGS__)
	#define LOG_ERROR(log, fmt, ...)		log.writelog(rtspsvr::Logger::LogError, __func__, __LINE__, fmt, ##__VA_ARGS__)
}
