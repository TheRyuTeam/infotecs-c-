#include "src/ServerApplication.h"

int main()
{
    const SocketAddress address("127.0.0.1", 65535);
    class ServerApplication app(address);
    app.run();
    return 0;
}
