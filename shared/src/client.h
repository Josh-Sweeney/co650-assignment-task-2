#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>
#include <iostream>

class Client
{
private:
    SOCKET clientSocket;
    sockaddr_in clientService;

    int connectSocket();

public:
    int initialize();
};
