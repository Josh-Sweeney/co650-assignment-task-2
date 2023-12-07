#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>
#include <iostream>

// TODO: Supposed to be abstract
class Comms
{
public:
    static int initializeWinsock();
    static int createSocket(SOCKET &outSocket);
    static int createService(sockaddr_in& service);
};