#include <errno.h>
#include "platformcommon.h"

namespace rtspsvr
{
    int initializeSock()
    {
#if defined(_WIN32) || defined(WIN32)
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
#endif
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