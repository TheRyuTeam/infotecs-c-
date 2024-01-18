#pragma once
#include "net.h"
#include <string>
#include <cstdint>

NET_START

class SocketAddress
{
public:
    SocketAddress();
    SocketAddress(const char* addr, unsigned short port);
	SocketAddress(const std::string& address, unsigned short port);
	SocketAddress(const SocketAddress&) = default;
	SocketAddress(SocketAddress&&) noexcept = default;

	SocketAddress& operator=(const SocketAddress&) = default;
	SocketAddress& operator=(SocketAddress&&) noexcept = default;

	int family() const noexcept;
	unsigned short port() const noexcept;

    const void* addr() const noexcept;
	int len() const noexcept;

	std::string toString() const;

	int cmp(const SocketAddress& other) const noexcept;

    static SocketAddress fromBuiltIn(const void* addr, int len);

private:
    uint8_t _addr[16];
};

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

NET_END
