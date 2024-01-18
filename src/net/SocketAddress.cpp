#include "SocketAddress.h"
#include "NetException.h"
#include "privatedefs.h"
#include <cstring>

#define AS_SOCKADDR(x) ((sockaddr*)(&(x)))
#define AS_SOCKADDR_IN(x) ((sockaddr_in*)(&(x)))
#define AS_SOCKADDR_IN6(x) ((sockaddr_in6*)(&(x)))

NET_START

SocketAddress::SocketAddress()
    : _addr()
{
}

SocketAddress::SocketAddress(const char* addr, unsigned short port)
{
    if (inet_pton(AF_INET, addr, &AS_SOCKADDR_IN(_addr)->sin_addr) == 1) {
        AS_SOCKADDR(_addr)->sa_family = AF_INET;

        auto pCast = ((sockaddr_in*)&_addr);
        pCast->sin_port = htons(port);
    } else if (inet_pton(AF_INET6, addr, &AS_SOCKADDR_IN6(_addr)->sin6_addr) == 1) {
        AS_SOCKADDR(_addr)->sa_family = AF_INET6;

        auto pCast = ((sockaddr_in6*)&_addr);
        pCast->sin6_port = htons(port);
        pCast->sin6_flowinfo = 0;
        pCast->sin6_scope_id = 0;
    } else {
        throw InvalidAddress(std::string("Invalid address \"") + addr + "\"", 0);
    }
}

SocketAddress::SocketAddress(const std::string& address, unsigned short port)
    : SocketAddress(address.c_str(), port)
{
}

int SocketAddress::family() const noexcept
{
    return AS_SOCKADDR(_addr)->sa_family;
}

unsigned short SocketAddress::port() const noexcept
{
    return ntohs(family() == AF_INET ? ((sockaddr_in*)&_addr)->sin_port : ((sockaddr_in6*)&_addr)->sin6_port);
}

const void* SocketAddress::addr() const noexcept
{
    return &_addr;
}

int SocketAddress::len() const noexcept
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

std::string SocketAddress::toString() const
{
    char buffer[128] {};
    if (family() == AF_INET) {
        inet_ntop(family(), &AS_SOCKADDR_IN(_addr)->sin_addr, buffer, sizeof(buffer));
        return std::string(buffer) + ':' + std::to_string(port());
    }

    inet_ntop(family(), &AS_SOCKADDR_IN6(_addr)->sin6_addr, buffer, sizeof(buffer));
    return '[' + std::string(buffer) + "]:" + std::to_string(port());
}

int SocketAddress::cmp(const SocketAddress& other) const noexcept
{
    if (family() != other.family()) {
        return family() < other.family() ? -1 : 1;
    }

    if (family() == AF_INET) {
        int diff = memcmp(&AS_SOCKADDR_IN(_addr)->sin_addr, &AS_SOCKADDR_IN(other._addr)->sin_addr, sizeof(in_addr));
        if (diff != 0) {
            return diff < 0 ? -1 : 1;
        }
    } else {
        int diff = memcmp(&AS_SOCKADDR_IN6(_addr)->sin6_addr, &AS_SOCKADDR_IN6(other._addr)->sin6_addr, sizeof(in6_addr));
        if (diff != 0) {
            return diff < 0 ? -1 : 1;
        }
    }

    auto ownPort = port();
    auto otherPort = other.port();
    if (ownPort != otherPort) {
        return ownPort < otherPort ? -1 : 1;
    }

    return 0;
}

SocketAddress SocketAddress::fromBuiltIn(const void* addr, int len)
{
    SocketAddress address;
    if (len > sizeof(sockaddr)) {
        throw InvalidAddress("Size of sockaddr can not exceed " + std::to_string(sizeof(sockaddr)) + " bytes", 0);
    }

    memcpy(&address._addr, addr, len);
    return address;
}

NET_END
