#include "unixdefs.h"
#include "net.h"

#ifdef _UNIX
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
    return errno
}

socket_t openSocket(int domain, int type, int protocol)
{
    return ::socket(domain, type, protocol);
}

socket_t closeSocket(socket_t socket)
{
    close(socket);
}

NET_END
#endif
