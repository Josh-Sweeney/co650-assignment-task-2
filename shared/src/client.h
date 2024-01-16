#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>
#include <iostream>
#include <string>

class Client
{
private:
    SOCKET clientSocket;
    sockaddr_in clientService;

    static DWORD WINAPI sendThread(LPVOID param);

    void connectSocket();

public:
    void run();
    void shutdown();
};
