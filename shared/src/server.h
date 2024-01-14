#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>
#include <iostream>
#include <windows.h>

class Server {
    private:
        SOCKET serverSocket;
        SOCKET acceptSocket;
        sockaddr_in serverService;

        static DWORD WINAPI acceptThread(LPVOID param);
        static DWORD WINAPI receiveThread(LPVOID param);

        int bindSocket();
        int listenSocket();

    public:
        int initialize();
        void shutdown();
};
