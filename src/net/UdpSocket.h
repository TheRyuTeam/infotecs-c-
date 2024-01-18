#pragma once
#include "SocketAddress.h"
#include "defs.h"

NET_START

class UdpSocket
{
public:
    UdpSocket(socket_t socket = kInvalidSocket);
    UdpSocket(int domain, int protocol);

    ~UdpSocket();

    void bind(const SocketAddress& address);

    int sendBytesTo(const char* data, int size, const SocketAddress& address);
    int recieveBytesFrom(char* data, int maxSize, SocketAddress& address);

	static void error(int code, const std::string& arg = "");

private:
	socket_t _socket;
	bool _blocking;
};

NET_END
