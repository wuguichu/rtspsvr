#pragma once

#if defined(_WIN32) || defined(WIN32)
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#if defined(_WIN32) || defined(WIN32)
#else
#define closesocket close
#endif


namespace rtspsvr
{
int initializeSock();
int getErrno();
} // namespace rtspsvr