#include "ClientApplication.h"
#include <iostream>

ClientApplication::ClientApplication(const net::SocketAddress& address, const IErrorProcessor::Ptr& errorProcessor)
    : _address(address)
    , _socket(address.family(), 0)
    , _senderThread()
    , _mutex()
    , _condition()
    , _buffer()
    , _errorProcessor(errorProcessor)
    , _maxSize(64)
    , _working(false)
{
}

ClientApplication::~ClientApplication()
{
    stop();
}

void ClientApplication::run()
{
    _working = true;
    _senderThread = std::thread(&ClientApplication::senderThreadFunc, this);
    inputThreadFunc();
}

void ClientApplication::stop()
{
    _working = false;
    _condition.notify_all();
    if (_senderThread.joinable()) {
        _senderThread.join();
    }
}

void ClientApplication::inputThreadFunc()
{
    while (_working) {
        std::string buff;
        std::cin >> buff;
        processInput(buff);
    }
}

void ClientApplication::senderThreadFunc()
{
    while (true) {
        std::string buff;
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _condition.wait(lock);
            if (!_working) {
                break;
            }

            buff = _buffer;
        }

        if (!buff.empty()) {
            sendData(buff);
        }
    }
}

void ClientApplication::processInput(const std::string& buff)
{
    if (buff.size() > _maxSize) {
        error("String size limit exceed: given size " + std::to_string(buff.size()) +
                               " when maximum size is " + std::to_string(_maxSize));
        return;
    }

    int sortingBuff[10]{0};
    for (const auto& c : buff) {
        if (!isdigit(c)) {
            error("String must contain only digits");
            return;
        }
        ++sortingBuff[c - '0'];
    }

    std::string result;
    for (int i = 9; i > -1; --i) {
        auto count = sortingBuff[i];
        if (count != 0) {
            if (i % 2 == 0) {
                while (count--) {
                    result += "KB";
                }
            } else {
                result.insert(result.end(), count, i + '0');
            }
        }
    }

    if (!result.empty()) {
        _buffer = result;
        _condition.notify_one();
    }
}

void ClientApplication::sendData(const std::string& data)
{
    char serverResponse[2]{};
    int serverResponseLen = 0;
    net::SocketAddress addr;

    try {
        _socket.sendBytesTo(data.c_str(), data.size(), _address);
        serverResponseLen = _socket.recieveBytesFrom(serverResponse, sizeof(serverResponse), addr);
    } catch (...) {
    }

    if (addr != _address || serverResponseLen != 1 || serverResponse[0] != 'K') {
        error("Server does not response");
    }
}

void ClientApplication::error(const std::string& msg)
{
    if (_errorProcessor) {
        _errorProcessor->error(msg);
    }
}
