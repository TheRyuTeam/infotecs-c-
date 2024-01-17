#pragma once
#include "net/UdpSocket.h"

class ServerApplication
{
public:
    ServerApplication(const SocketAddress& address);

    void run();
    void stop();

private:
    void receiverThreadFunc();

private:
    UdpSocket _socket;
    bool _working;
};
