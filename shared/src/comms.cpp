#include "comms.h"

static int port = 55555;
static const char *localhostAddress = _T("127.0.0.1");

void Comms::initializeWinsock()
{
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 2);

    int wsaError = WSAStartup(wVersionRequested, &wsaData);

    if (wsaError != 0)
    {
        throw std::runtime_error("The Winsock dll was not found");
    }

    std::cout << "The Winsock dll was found" << std::endl;
    std::cout << "Status: " << wsaData.szSystemStatus << std::endl;
}

void Comms::createSocket(SOCKET &outSocket)
{
    SOCKET newSocket = INVALID_SOCKET;

    newSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (newSocket == INVALID_SOCKET)
    {
        throw std::runtime_error("Error creating Socket: " + WSAGetLastError());
    }

    std::cout << "Created Socket" << std::endl;

    outSocket = newSocket;
}

void Comms::createService(sockaddr_in &service)
{
    service.sin_family = AF_INET;

    InetPton(AF_INET, localhostAddress, &service.sin_addr.s_addr);
    service.sin_port = htons(port);
}