#include "client.h"

int main()
{
    Client client{};

    try
    {
        // Client::run() starts a thread and waits for it to finish.
        client.run();
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
        client.shutdown();
        return -1;
    }

    system("pause");

    return 0;
}
