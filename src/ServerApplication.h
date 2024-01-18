#pragma once
#include "net/UdpSocket.h"

class ServerApplication
{
public:
    ServerApplication(const net::SocketAddress& address);

    void run();
    void stop();

private:
    void receiverThreadFunc();

private:
    net::UdpSocket _socket;
    bool _working;
};
