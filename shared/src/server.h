#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>
#include <iostream>

class Server {
    public:
        int bindSocket(SOCKET& serverSocket, sockaddr_in& serverService);
};