#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>
#include <iostream>

// Abstract class
class Comms
{
public:
    static void initializeWinsock();
    static void createSocket(SOCKET &outSocket);
    static void createService(sockaddr_in& service);
};