#include "UdpSocket.h"
#include "NetException.h"
#include "privatedefs.h"

NET_START

UdpSocket::UdpSocket(socket_t socket)
	: _socket(socket)
	, _blocking(true)
{
}

UdpSocket::UdpSocket(int domain, int protocol)
    : UdpSocket(net::openSocket(domain, SOCK_DGRAM, protocol))
{
}

UdpSocket::~UdpSocket()
{
    net::closeSocket(_socket);
    _socket = kInvalidSocket;
}

void UdpSocket::bind(const SocketAddress& address)
{
    if (_socket == kInvalidSocket) {
        _socket = net::openSocket(address.family(), SOCK_DGRAM, 0);
    }

    if (::bind(_socket, (const sockaddr*)address.addr(), address.len()) != 0) {
        error(net::error(), address.toString());
	}
}

int UdpSocket::sendBytesTo(const char* data, int size, const SocketAddress& address)
{
	int rc;
	do {
        rc = ::sendto(_socket, data, size, 0, (const sockaddr*)address.addr(), address.len());
    } while (rc < 0 && net::error() == NET_EINTR);

	if (rc < 0) {
        int err = net::error();
        if (err != NET_EAGAIN) {
			error(err);
		}
	}

	return rc;
}

int UdpSocket::recieveBytesFrom(char* data, int maxSize, SocketAddress& address)
{
    sockaddr addr{};
    int len = sizeof(sockaddr);
	int rc;
	do {
        rc = ::recvfrom(_socket, data, maxSize, 0, &addr, &len);
    } while (rc < 0 && net::error() == NET_EINTR);

	if (rc < 0) {
        int err = net::error();
        if (err != NET_EAGAIN) {
			error(err);
		}
	}

    address = SocketAddress::fromBuiltIn(&addr, len);
    return rc;
}

void UdpSocket::error(int code, const std::string& arg)
{
	switch (code)
	{
		case 0:
			return;
        case NET_ESYSNOTREADY:
			throw NetException("Net subsystem not ready", code);
        case NET_NOTINITIALISED:
			throw NetException("Net subsystem not initialized", code);
        case NET_EINTR:
			throw IOException("Interrupted", code);
        case NET_EACCES:
			throw IOException("Permission denied", code);
        case NET_EFAULT:
			throw IOException("Bad address", code);
        case NET_EINVAL:
			throw InvalidArgumentException("Invalid argument", code);
        case NET_EMFILE:
			throw IOException("Too many open files", code);
        case NET_EWOULDBLOCK:
			throw IOException("Operation would block", code);
        case NET_EINPROGRESS:
			throw IOException("Operation now in progress", code);
        case NET_EALREADY:
			throw IOException("Operation already in progress", code);
        case NET_ENOTSOCK:
			throw IOException("Socket operation attempted on non-socket", code);
        case NET_EDESTADDRREQ:
			throw NetException("Destination address required", code);
        case NET_EMSGSIZE:
			throw NetException("Message too long", code);
        case NET_EPROTOTYPE:
			throw NetException("Wrong protocol type", code);
        case NET_ENOPROTOOPT:
			throw NetException("Protocol not available", code);
        case NET_EPROTONOSUPPORT:
			throw NetException("Protocol not supported", code);
        case NET_ESOCKTNOSUPPORT:
			throw NetException("Socket type not supported", code);
        case NET_ENOTSUP:
			throw NetException("Operation not supported", code);
        case NET_EPFNOSUPPORT:
			throw NetException("Protocol family not supported", code);
        case NET_EAFNOSUPPORT:
			throw NetException("Address family not supported", code);
        case NET_EADDRINUSE:
			throw NetException(arg + " already in use", code);
        case NET_EADDRNOTAVAIL:
			throw NetException("Cannot assign " + arg, code);
        case NET_ENETDOWN:
			throw NetException("Network is down", code);
        case NET_ENETUNREACH:
			throw NetException("Network is unreachable", code);
        case NET_ENETRESET:
			throw NetException("Network dropped connection on reset", code);
        case NET_ECONNABORTED:
			throw ConnectionAbortedException("Connection aborted", code);
        case NET_ECONNRESET:
			throw ConnectionResetException("Connection reset", code);
        case NET_ENOBUFS:
			throw IOException("No buffer space available", code);
        case NET_EISCONN:
			throw NetException("Socket is already connected", code);
        case NET_ENOTCONN:
			throw NetException("Socket is not connected", code);
        case NET_ESHUTDOWN:
			throw NetException("Cannot send after socket shutdown", code);
        case NET_ETIMEDOUT:
			throw TimeoutException("Timeout", code);
        case NET_ECONNREFUSED:
			throw ConnectionRefusedException(arg, code);
        case NET_EHOSTDOWN:
			throw NetException("Host is down(" + arg + ')', code);
        case NET_EHOSTUNREACH:
			throw NetException("No route to host(" + arg + ')', code);
#if defined(NET_OS_FAMILY_UNIX)
		case EPIPE:
			throw IOException("Broken pipe", code);
		case EBADF:
			throw IOException("Bad socket descriptor", code);
		case ENOENT:
			throw IOException("Not found", arg, code);
#endif
		default:
			throw IOException(std::to_string(code) + '(' + arg + ')', code);
	}
}

NET_END
