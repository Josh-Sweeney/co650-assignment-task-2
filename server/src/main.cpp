#include "server.h"

int main() {
    Server server;
    
    if (server.initialize() != 0)
        server.shutdown();

    system("pause");
    
    return 0;
}