#include "server.h"

int main()
{
    Server server{};

    try
    {
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
