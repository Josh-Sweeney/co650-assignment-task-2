#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>
#include <iostream>

class Client
{
private:
    SOCKET clientSocket;
    sockaddr_in clientService;

    static DWORD WINAPI sendThread(LPVOID param);

    int connectSocket();

public:
    int initialize();
    void shutdown();
};
