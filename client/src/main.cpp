#include "client.h"

int main() {
    Client client;
    
    if (client.initialize() != 0)
        client.shutdown();
    
    system("pause");

    return 0;
}