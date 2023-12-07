#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>
#include <iostream>

class Client
{
public:
    int Client::connectSocket(SOCKET &clientSocket, sockaddr_in &clientService);
};
