#include "platformcommon.h"
#if defined(_WIN32) || defined(WIN32)
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <errno.h>
#endif

namespace rtspsvr
{
int initializeSock()
{
    static bool bIsInit = false;
    if (!bIsInit)
    {
        WSADATA wsadata;
        if ((WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) && ((WSAStartup(MAKEWORD(1, 1), &wsadata)) != 0))
        {
            return false;
        }
        if ((wsadata.wVersion != MAKEWORD(2, 2)) && (wsadata.wVersion != MAKEWORD(1, 1)))
        {
            WSACleanup();
            return false;
        }
        bIsInit = true;
    }
    return true;
}

int getErrno()
{
#if defined(_WIN32) || defined(WIN32)
    return GetLastError();
#else
    return errno;
#endif
}
} // namespace rtspsvr