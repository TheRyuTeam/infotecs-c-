#include "windefs.h"
#include "net.h"
#include "NetException.h"
#include "privatedefs.h"

#ifdef _WIN32

NET_START

int error()
{
    return WSAGetLastError();
}

void initialize()
{
#ifdef _WIN32
    WSAData data;
    if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
        throw WSAInitError("Can not initialize wsa", error());
    }
#endif
}

void uninitialize()
{
#ifdef _WIN32
    WSACleanup();
#endif
}

socket_t openSocket(int domain, int type, int protocol)
{
    initialize();
    return ::socket(domain, type, protocol);
}

void closeSocket(socket_t socket)
{
    uninitialize();
    closesocket(socket);
}

NET_END

#endif
