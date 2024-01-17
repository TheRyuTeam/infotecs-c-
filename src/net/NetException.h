#pragma once
#include <stdexcept>
#include <string>

class IOException: public std::exception
{
public:
	explicit IOException(const std::string& msg, int code)
		: _msg(msg + (code != 0 ? "\nError code: " + std::to_string(code) : ""))
		, _code(code)
	{
	}

	const char* what() const noexcept override
	{
		return _msg.c_str();
	}

private:
	std::string _msg;
	int _code;
};

class NetError: public std::exception
{
public:
	explicit NetError(const std::string& msg, int code)
		: _msg(msg + (code != 0 ? "\nError code: " + std::to_string(code) : ""))
		, _code(code)
	{
	}

	const char* what() const noexcept override
	{
		return _msg.c_str();
	}

private:
	std::string _msg;
	int _code;
};

#define SIMPLE_EXCEPTION(Class, Parent) \
class Class : public Parent\
{\
public:\
	using Parent::Parent;\
}

#ifdef WIN32
SIMPLE_EXCEPTION(WSAInitError, NetError);
SIMPLE_EXCEPTION(WSACleanupError, NetError);
#endif

SIMPLE_EXCEPTION(InvalidSocket, NetError);
SIMPLE_EXCEPTION(InvalidArgumentException, NetError);
SIMPLE_EXCEPTION(AddressError, NetError);
SIMPLE_EXCEPTION(InvalidAddress, AddressError);
SIMPLE_EXCEPTION(AddressParseError, AddressError);

SIMPLE_EXCEPTION(NetException, NetError);
SIMPLE_EXCEPTION(InvalidAddressException, NetException);
SIMPLE_EXCEPTION(InvalidSocketException, NetException);
SIMPLE_EXCEPTION(ServiceNotFoundException, NetException);
SIMPLE_EXCEPTION(ConnectionAbortedException, NetException);
SIMPLE_EXCEPTION(ConnectionResetException, NetException);
SIMPLE_EXCEPTION(ConnectionRefusedException, NetException);
SIMPLE_EXCEPTION(DNSException, NetException);
SIMPLE_EXCEPTION(HostNotFoundException, DNSException);
SIMPLE_EXCEPTION(NoAddressFoundException, DNSException);
SIMPLE_EXCEPTION(InterfaceNotFoundException, NetException);
SIMPLE_EXCEPTION(NoMessageException, NetException);
SIMPLE_EXCEPTION(MessageException, NetException);
SIMPLE_EXCEPTION(MultipartException, MessageException);
SIMPLE_EXCEPTION(HTTPException, NetException);
SIMPLE_EXCEPTION(NotAuthenticatedException, HTTPException);
SIMPLE_EXCEPTION(UnsupportedRedirectException, HTTPException);
SIMPLE_EXCEPTION(FTPException, NetException);
SIMPLE_EXCEPTION(SMTPException, NetException);
SIMPLE_EXCEPTION(POP3Exception, NetException);
SIMPLE_EXCEPTION(ICMPException, NetException);
SIMPLE_EXCEPTION(NTPException, NetException);
SIMPLE_EXCEPTION(HTMLFormException, NetException);
SIMPLE_EXCEPTION(WebSocketException, NetException);
SIMPLE_EXCEPTION(UnsupportedFamilyException, NetException);
SIMPLE_EXCEPTION(AddressFamilyMismatchException, NetException);
SIMPLE_EXCEPTION(TimeoutException, NetException);

SIMPLE_EXCEPTION(SockOptException, NetException);

#undef SIMPLE_EXCEPTION