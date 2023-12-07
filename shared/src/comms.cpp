#include "comms.h"

// returns -1 on error, 0 on success
int Comms::initializeWinsock() {
    int port = 55555;

    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 2);

    int wsaError = WSAStartup(wVersionRequested, &wsaData);
    
    if (wsaError != 0) {
        std::cout << "The Winsock dll was not found" << std::endl;
        return -1;
    }

    std::cout << "The Winsock dll was found" << std::endl;
    std::cout << "Status: " << wsaData.szSystemStatus << std::endl;

    return 0;
}

// returns -1 on error, 0 on success
// socket is a reference to a SOCKET
int Comms::createSocket(SOCKET& outSocket) {
    SOCKET newSocket = INVALID_SOCKET;
    newSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (newSocket == INVALID_SOCKET) {
        std::cout << "Error creating Socket: " << std::endl;
        std::cout << WSAGetLastError() << std::endl;
        
        WSACleanup();
        return -1;
    }
    
    std::cout << "Created Socket" << std::endl;

    outSocket = newSocket;

    return 0;
}