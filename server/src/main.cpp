#include "server.h"

int main()
{
    Server server{};

    try
    {
        server.initialize();
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
