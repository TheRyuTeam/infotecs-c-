#include "SocketAddress.h"
#include "NetException.h"
#include <ws2tcpip.h>

SocketAddress::SocketAddress()
    : _addr()
{
}

SocketAddress::SocketAddress(const sockaddr* addr, int len)
    : SocketAddress()
{
    if (len > sizeof(sockaddr)) {
        throw InvalidAddress("Size of sockaddr can not exceed " + std::to_string(sizeof(sockaddr)) + " bytes", 0);
    }

    memcpy(&_addr, addr, len);
}

SocketAddress::SocketAddress(uint32_t addr, unsigned short port)
    : _addr()
{
    _addr.sa_family = AF_INET;
    auto pCast = asV4();
    pCast->sin_port = htons(port);
    pCast->sin_addr.S_un.S_addr = addr;
}

SocketAddress::SocketAddress(const char* address, unsigned short port)
    : _addr()
{
    if (inet_pton(AF_INET, address, &asV4()->sin_addr) == 1) {
        _addr.sa_family = AF_INET;

        auto pCast = ((sockaddr_in*)&_addr);
        pCast->sin_port = htons(port);
    } else if (inet_pton(AF_INET6, address, &asV6()->sin6_addr) == 1) {
        _addr.sa_family = AF_INET6;

        auto pCast = ((sockaddr_in6*)&_addr);
        pCast->sin6_port = htons(port);
        pCast->sin6_flowinfo = 0;
        pCast->sin6_scope_id = 0;
    } else {
        throw InvalidAddress(std::string("Invalid address \"") + address + "\"", 0);
    }
}

SocketAddress::SocketAddress(const std::string& address, unsigned short port)
    : SocketAddress(address.c_str(), port)
{
}

std::string SocketAddress::toString() const
{
    char buffer[128] {};
    if (family() == AF_INET) {
        inet_ntop(family(), &asV4()->sin_addr, buffer, sizeof(buffer));
        return std::string(buffer) + ':' + std::to_string(port());
    }

    inet_ntop(family(), &asV6()->sin6_addr, buffer, sizeof(buffer));
    return '[' + std::string(buffer) + "]:" + std::to_string(port());
}

int SocketAddress::cmp(const SocketAddress& other) const noexcept
{
    if (family() != other.family()) {
        return family() < other.family() ? -1 : 1;
    }

    if (family() == AF_INET) {
        int diff = memcmp(&asV4()->sin_addr, &other.asV4()->sin_addr, sizeof(in_addr));
        if (diff != 0) {
            return diff < 0 ? -1 : 1;
        }
    } else {
        int diff = memcmp(&asV6()->sin6_addr, &other.asV6()->sin6_addr, sizeof(in6_addr));
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
