#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>
#include <iostream>

class Server {
    private:
        SOCKET serverSocket;
        sockaddr_in serverService;

        int bindSocket();
        int Server::listenSocket();

    public:
        int initialize();
};