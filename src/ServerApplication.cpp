#include "ServerApplication.h"
#include <iostream>

ServerApplication::ServerApplication(const SocketAddress& address)
    : _socket()
    , _working(false)
{
    _socket.bind(address);
}

void ServerApplication::run()
{
    _working = true;
    receiverThreadFunc();
}

void ServerApplication::stop()
{
    _working = false;
}

void ServerApplication::receiverThreadFunc()
{
    while (_working) {
        char data[128]{};
        SocketAddress address;
        int rc = _socket.recieveBytesFrom(data, sizeof(data), address);
        _socket.sendBytesTo("K", 1, address);
        if (rc % 2 == 0 && rc % 32 == 0) {
            std::cout << "Data received\n";
        }
    }
}
