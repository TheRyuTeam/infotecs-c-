#include "unixdefs.h"
#include "privatedefs.h"

#ifndef _WIN32
#include <cerrno>

NET_START

void initialize()
{
}

void uninitialize()
{
}

int error()
{
    return errno;
}

socket_t openSocket(int domain, int type, int protocol)
{
    return ::socket(domain, type, protocol);
}

void closeSocket(socket_t socket)
{
    close(socket);
}

NET_END
#endif
