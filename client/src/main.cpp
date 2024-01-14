#include "client.h"

int main()
{
    Client client{};

    try
    {
        client.initialize();
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
