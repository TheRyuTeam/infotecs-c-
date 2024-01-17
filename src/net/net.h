#pragma once

#ifndef NOMINMAX
#	define NOMINMAX 1
#endif

#ifdef _WIN32
#	define WINVER                  0x600
#	define _WIN32_WINDOWS          0x600
#	ifdef _WIN32_WINNT
#		undef _WIN32_WINNT
#	endif
#	define _WIN32_WINNT            0x600
#	include <Winsock2.h>
#	include <Ws2tcpip.h>

using socket_t = SOCKET;
inline int net_last_error()
{
	return WSAGetLastError();
}

#else
#	include <arpa/inet.h>
#   include <cerrno>

using socket_t = int;

inline int net_last_error()
{
    return errno;
}

#endif
