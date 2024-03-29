#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <windows.h>

class Server
{
private:
    SOCKET serverSocket;
    SOCKET acceptSocket;
    sockaddr_in serverService;

    bool shutdownRequested = false;

    static DWORD WINAPI acceptThread(LPVOID param);
    static DWORD WINAPI receiveThread(LPVOID param);

    void bindSocket();
    void listenSocket();

public:
    void run();
    void shutdown();
};
