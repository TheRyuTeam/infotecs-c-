#pragma once
#include "net.h"

#ifdef _WIN32
#   include "windefs.h"
#else
#   include "unixdefs.h"
#endif

NET_START

int error();
void initialize();
void uninitialize();

socket_t openSocket(int domain, int type, int protocol);
void closeSocket(socket_t socket);

NET_END
