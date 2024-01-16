#include "server.h"

int main()
{
    Server server{};

    try
    {
        // Server::run() starts a thread and waits for it to finish.
        server.run();
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
        server.shutdown();
        return -1;
    }

    system("pause");

    return 0;
}
