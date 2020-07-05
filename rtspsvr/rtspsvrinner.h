#pragma once

#include "platformcommon.h"
#include "logger.h"

namespace rtspsvr
{

#define RTSP_LOG_LEVEL rtspsvr::Logger::LogDebug

#define RTSP_DEBUG(fmt, ...)                                                                                                       \
    do                                                                                                                             \
    {                                                                                                                              \
        if (rtspsvr::Logger::LogDebug >= RTSP_LOG_LEVEL)                                                                           \
        {                                                                                                                          \
            Logger::getInstance()->writelog("%s", Logger::getInstance()->getPrefix(Logger::LogDebug, __func__, __LINE__).c_str()); \
            Logger::getInstance()->writelog(fmt, ##__VA_ARGS__);                                                                   \
        }                                                                                                                          \
    } while (0)
#define RTSP_INFO(fmt, ...)                                                                                                       \
    do                                                                                                                            \
    {                                                                                                                             \
        if (rtspsvr::Logger::LogInfo >= RTSP_LOG_LEVEL)                                                                           \
        {                                                                                                                         \
            Logger::getInstance()->writelog("%s", Logger::getInstance()->getPrefix(Logger::LogInfo, __func__, __LINE__).c_str()); \
            Logger::getInstance()->writelog(fmt, ##__VA_ARGS__);                                                                  \
        }                                                                                                                         \
    } while (0)
#define RTSP_WARN(fmt, ...)                                                                                                       \
    do                                                                                                                            \
    {                                                                                                                             \
        if (rtspsvr::Logger::LogWarn >= RTSP_LOG_LEVEL)                                                                           \
        {                                                                                                                         \
            Logger::getInstance()->writelog("%s", Logger::getInstance()->getPrefix(Logger::LogWarn, __func__, __LINE__).c_str()); \
            Logger::getInstance()->writelog(fmt, ##__VA_ARGS__);                                                                  \
        }                                                                                                                         \
    } while (0)
#define RTSP_ERROR(fmt, ...)                                                                                                       \
    do                                                                                                                             \
    {                                                                                                                              \
        if (rtspsvr::Logger::LogError >= RTSP_LOG_LEVEL)                                                                           \
        {                                                                                                                          \
            Logger::getInstance()->writelog("%s", Logger::getInstance()->getPrefix(Logger::LogError, __func__, __LINE__).c_str()); \
            Logger::getInstance()->writelog(fmt, ##__VA_ARGS__);                                                                   \
        }                                                                                                                          \
    } while (0)

//Use stream format,Can't use std::endl yet
#define RTSP_DEBUG_S                                 \
    if (rtspsvr::Logger::LogDebug >= RTSP_LOG_LEVEL) \
    *Logger::getInstance() << Logger::getInstance()->getPrefix(Logger::LogDebug, __func__, __LINE__).c_str()
#define RTSP_INFO_S                                 \
    if (rtspsvr::Logger::LogInfo >= RTSP_LOG_LEVEL) \
    *Logger::getInstance() << Logger::getInstance()->getPrefix(Logger::LogInfo, __func__, __LINE__).c_str()
#define RTSP_WARN_S                                 \
    if (rtspsvr::Logger::LogWarn >= RTSP_LOG_LEVEL) \
    *Logger::getInstance() << Logger::getInstance()->getPrefix(Logger::LogWarn, __func__, __LINE__).c_str()
#define RTSP_ERROR_S                                 \
    if (rtspsvr::Logger::LogError >= RTSP_LOG_LEVEL) \
    *Logger::getInstance() << Logger::getInstance()->getPrefix(Logger::LogError, __func__, __LINE__).c_str()
} // namespace rtspsvr