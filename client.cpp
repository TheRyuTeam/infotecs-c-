#include "src/ClientApplication.h"
#include <iostream>

class ErrorProcessor
    : public IErrorProcessor
{
public:
    void error(const std::string& msg) override
    {
        std::cerr << msg << '\n';
    }
};

int main()
{
    const net::SocketAddress address("127.0.0.1", 65535);
    ClientApplication app(address, IErrorProcessor::Ptr(new ErrorProcessor));
    app.run();
    return 0;
}
