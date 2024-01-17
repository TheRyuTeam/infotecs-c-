#include "UdpSocket.h"
#include "NetException.h"
#include <atomic>

#ifdef _WIN32
static std::atomic<std::size_t> WSA_INIT_COUNTER;
static WSAData WSA_DATA;

void wsaStart()
{
	if (WSA_INIT_COUNTER++ > 0) {
		return;
	}

	if (WSAStartup(MAKEWORD(2, 2), &WSA_DATA) != 0) {
		throw WSAInitError("Can not initialize wsa", net_last_error());
	}
}

void wsaStop()
{
	if (--WSA_INIT_COUNTER >= 0) {
		return;
	}

	if (WSACleanup() != 0) {
		throw WSACleanupError("Can not clean wsa", net_last_error());
	}
}

socket_t netOpenSocket(int domain, int type, int protocol)
{
	wsaStart();
	return ::socket(domain, type, protocol);
}

void netCloseSocket(socket_t socket)
{
	if (socket == INVALID_SOCKET) {
		return;
	}

	closesocket(socket);
	wsaStop();
}
#else
socket_t netOpenSocket(int domain, int type, int protocol)
{
    return ::socket(domain, type, protocol);
}

void netCloseSocket(socket_t socket)
{
    if (socket == INVALID_SOCKET) {
        return;
    }

    close(socket);
}

#endif


UdpSocket::UdpSocket(socket_t socket)
	: _socket(socket)
	, _blocking(true)
{
}

UdpSocket::UdpSocket(int domain, int protocol)
    : UdpSocket(netOpenSocket(domain, SOCK_DGRAM, protocol))
{
}

UdpSocket::~UdpSocket()
{
    netCloseSocket(_socket);
    _socket = INVALID_SOCKET;
}

void UdpSocket::bind(const SocketAddress& address)
{
    if (_socket == INVALID_SOCKET) {
        _socket = netOpenSocket(address.family(), SOCK_DGRAM, 0);
    }

	if (::bind(_socket, address.addr(), address.len()) != 0) {
		error(net_last_error(), address.toString());
	}
}

int UdpSocket::sendBytesTo(const char* data, int size, const SocketAddress& address)
{
	int rc;
	do {
        rc = ::sendto(_socket, data, size, 0, address.addr(), address.len());
    } while (rc < 0 && net_last_error() == WSAEINTR);

	if (rc < 0) {
		int err = net_last_error();
        if (err != WSAEWOULDBLOCK) {
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
    } while (rc < 0 && net_last_error() == WSAEINTR);

	if (rc < 0) {
		int err = net_last_error();
        if (err != WSAEWOULDBLOCK) {
			error(err);
		}
	}

    address = SocketAddress(&addr, len);
    return rc;
}

void UdpSocket::error(int code, const std::string& arg)
{
	switch (code)
	{
		case 0:
			return;
        case WSASYSNOTREADY:
			throw NetException("Net subsystem not ready", code);
        case WSANOTINITIALISED:
			throw NetException("Net subsystem not initialized", code);
        case WSAEINTR:
			throw IOException("Interrupted", code);
        case WSAEACCES:
			throw IOException("Permission denied", code);
        case WSAEFAULT:
			throw IOException("Bad address", code);
        case WSAEINVAL:
			throw InvalidArgumentException("Invalid argument", code);
        case WSAEMFILE:
			throw IOException("Too many open files", code);
        case WSAEWOULDBLOCK:
			throw IOException("Operation would block", code);
        case WSAEINPROGRESS:
			throw IOException("Operation now in progress", code);
        case WSAEALREADY:
			throw IOException("Operation already in progress", code);
        case WSAENOTSOCK:
			throw IOException("Socket operation attempted on non-socket", code);
        case WSAEDESTADDRREQ:
			throw NetException("Destination address required", code);
        case WSAEMSGSIZE:
			throw NetException("Message too long", code);
        case WSAEPROTOTYPE:
			throw NetException("Wrong protocol type", code);
        case WSAENOPROTOOPT:
			throw NetException("Protocol not available", code);
        case WSAEPROTONOSUPPORT:
			throw NetException("Protocol not supported", code);
        case WSAESOCKTNOSUPPORT:
			throw NetException("Socket type not supported", code);
        case WSAEOPNOTSUPP:
			throw NetException("Operation not supported", code);
        case WSAEPFNOSUPPORT:
			throw NetException("Protocol family not supported", code);
        case WSAEAFNOSUPPORT:
			throw NetException("Address family not supported", code);
        case WSAEADDRINUSE:
			throw NetException(arg + " already in use", code);
        case WSAEADDRNOTAVAIL:
			throw NetException("Cannot assign " + arg, code);
        case WSAENETDOWN:
			throw NetException("Network is down", code);
        case WSAENETUNREACH:
			throw NetException("Network is unreachable", code);
        case WSAENETRESET:
			throw NetException("Network dropped connection on reset", code);
        case WSAECONNABORTED:
			throw ConnectionAbortedException("Connection aborted", code);
        case WSAECONNRESET:
			throw ConnectionResetException("Connection reset", code);
        case WSAENOBUFS:
			throw IOException("No buffer space available", code);
        case WSAEISCONN:
			throw NetException("Socket is already connected", code);
        case WSAENOTCONN:
			throw NetException("Socket is not connected", code);
        case WSAESHUTDOWN:
			throw NetException("Cannot send after socket shutdown", code);
        case WSAETIMEDOUT:
			throw TimeoutException("Timeout", code);
        case WSAECONNREFUSED:
			throw ConnectionRefusedException(arg, code);
        case WSAEHOSTDOWN:
			throw NetException("Host is down(" + arg + ')', code);
        case WSAEHOSTUNREACH:
			throw NetException("No route to host(" + arg + ')', code);
#if defined(WSAOS_FAMILY_UNIX)
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
