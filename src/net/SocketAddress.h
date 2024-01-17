#pragma once
#include "net.h"
#include <string>

class SocketAddress
{
public:
	SocketAddress();
    SocketAddress(const sockaddr* addr, int len);
	SocketAddress(const char* address, unsigned short port);
	SocketAddress(const std::string& address, unsigned short port);
	SocketAddress(const SocketAddress&) = default;
	SocketAddress(SocketAddress&&) noexcept = default;

	SocketAddress& operator=(const SocketAddress&) = default;
	SocketAddress& operator=(SocketAddress&&) noexcept = default;

	int family() const noexcept;
	unsigned short port() const noexcept;

	const sockaddr* addr() const noexcept;
	int len() const noexcept;

	std::string toString() const;

	int cmp(const SocketAddress& other) const noexcept;

private:
	sockaddr_in* asV4() const noexcept;
	sockaddr_in6* asV6() const noexcept;

private:
	sockaddr _addr;
};

inline int SocketAddress::family() const noexcept
{
	return _addr.sa_family;
}

inline unsigned short SocketAddress::port() const noexcept
{
	return ntohs(family() == AF_INET ? ((sockaddr_in*)&_addr)->sin_port : ((sockaddr_in6*)&_addr)->sin6_port);
}

inline const sockaddr* SocketAddress::addr() const noexcept
{
	return &_addr;
}

inline int SocketAddress::len() const noexcept
{
	switch (family())
	{
	case AF_INET:
		return sizeof(sockaddr_in);

	case AF_INET6:
		return sizeof(sockaddr_in6);

	default:
		return -1;
	}
}

inline sockaddr_in* SocketAddress::asV4() const noexcept
{
	return (sockaddr_in*)&_addr;
}

inline sockaddr_in6* SocketAddress::asV6() const noexcept
{
	return (sockaddr_in6*)&_addr;
}


inline bool operator==(const SocketAddress& lhs, const SocketAddress& rhs)
{
	return lhs.cmp(rhs) == 0;
}

inline bool operator!=(const SocketAddress& lhs, const SocketAddress& rhs)
{
	return lhs.cmp(rhs) != 0;
}

inline bool operator>(const SocketAddress& lhs, const SocketAddress& rhs)
{
	return lhs.cmp(rhs) > 0;
}

inline bool operator<(const SocketAddress& lhs, const SocketAddress& rhs)
{
	return lhs.cmp(rhs) < 0;
}

inline bool operator>=(const SocketAddress& lhs, const SocketAddress& rhs)
{
	return lhs.cmp(rhs) >= 0;
}

inline bool operator<=(const SocketAddress& lhs, const SocketAddress& rhs)
{
	return lhs.cmp(rhs) <= 0;
}
