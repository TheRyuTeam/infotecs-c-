#pragma once
#include "net.h"

#include "windefs.h"
#include "unixdefs.h"

NET_START

int error();
void initialize();
void uninitialize();

socket_t openSocket(int domain, int type, int protocol);
void closeSocket(socket_t socket);

NET_END
