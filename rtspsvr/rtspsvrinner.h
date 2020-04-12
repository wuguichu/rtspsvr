#pragma once

#include "platformcommon.h"
#include "logger.h"

namespace rtspsvr
{

#define RTSP_DEBUG(fmt, ...)                                                                                          \
    do                                                                                                                \
    {                                                                                                                 \
        _log.writelog(rtspsvr::Logger::LogDebug, "%s", _log.getPrefix(Logger::LogDebug, __func__, __LINE__).c_str()); \
        _log.writelog(rtspsvr::Logger::LogDebug, fmt, ##__VA_ARGS__);                                                 \
    } while (0)
#define RTSP_INFO(fmt, ...)                                                                                         \
    do                                                                                                              \
    {                                                                                                               \
        _log.writelog(rtspsvr::Logger::LogInfo, "%s", _log.getPrefix(Logger::LogInfo, __func__, __LINE__).c_str()); \
        _log.writelog(rtspsvr::Logger::LogInfo, fmt, ##__VA_ARGS__);                                                \
    } while (0)
#define RTSP_WARN(fmt, ...)                                                                                         \
    do                                                                                                              \
    {                                                                                                               \
        _log.writelog(rtspsvr::Logger::LogWarn, "%s", _log.getPrefix(Logger::LogWarn, __func__, __LINE__).c_str()); \
        _log.writelog(rtspsvr::Logger::LogWarn, fmt, ##__VA_ARGS__);                                                \
    } while (0)
#define RTSP_ERROR(fmt, ...)                                                                                          \
    do                                                                                                                \
    {                                                                                                                 \
        _log.writelog(rtspsvr::Logger::LogError, "%s", _log.getPrefix(Logger::LogError, __func__, __LINE__).c_str()); \
        _log.writelog(rtspsvr::Logger::LogError, fmt, ##__VA_ARGS__);                                                 \
    } while (0)

//Use stream format,Can't use std::endl yet
#define RTSP_DEBUG_S                                  \
    if (rtspsvr::Logger::LogDebug >= _log.getLevel()) \
    _log << _log.getPrefix(Logger::LogDebug, __func__, __LINE__).c_str()
#define RTSP_INFO_S                                  \
    if (rtspsvr::Logger::LogInfo >= _log.getLevel()) \
    _log << _log.getPrefix(Logger::LogInfo, __func__, __LINE__).c_str()
#define RTSP_WARN_S                                  \
    if (rtspsvr::Logger::LogWarn >= _log.getLevel()) \
    _log << _log.getPrefix(Logger::LogWarn, __func__, __LINE__).c_str()
#define RTSP_ERROR_S                                  \
    if (rtspsvr::Logger::LogError >= _log.getLevel()) \
    _log << _log.getPrefix(Logger::LogError, __func__, __LINE__).c_str()
} // namespace rtspsvr