#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include "IErrorProcessor.h"
#include "net/UdpSocket.h"

class ClientApplication
{
public:
    ClientApplication(const SocketAddress& address, const IErrorProcessor::Ptr& errorProcessor);
    ~ClientApplication();

    void run();
    void stop();

private:
    void inputThreadFunc();
    void senderThreadFunc();
    void processInput(const std::string& buff);
    void sendData(const std::string& data);
    void error(const std::string& msg);

private:
    SocketAddress _address;
    UdpSocket _socket;
    std::thread _senderThread;
    std::mutex _mutex;
    std::condition_variable _condition;
    std::string _buffer;
    IErrorProcessor::Ptr _errorProcessor;
    std::size_t _maxSize;
    bool _working;
};
