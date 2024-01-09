#ifdef COMPILE_SERVER

#include "server.h"
#include "Comms.h"

DWORD WINAPI Server::acceptThread(LPVOID param)
{
    Server *instance = static_cast<Server *>(param);

    SOCKADDR_STORAGE from;
    int fromLength;
    char serverString[NI_MAXSERV], hostString[NI_MAXHOST];

    fromLength = sizeof(from);

    while (true)
    {
        instance->acceptSocket = accept(instance->serverSocket, (SOCKADDR *)&from, &fromLength);
        if (instance->acceptSocket == INVALID_SOCKET)
        {
            std::cout << "Server: Accept failed: " << std::endl;
            std::cout << WSAGetLastError() << std::endl;

            WSACleanup();
            return -1;
        }

        int getNameResult = getnameinfo((SOCKADDR *)&from, fromLength, hostString, NI_MAXHOST, serverString, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

        if (getNameResult != 0)
        {
            std::cout << "Server: Failed to get name info: " << std::endl;
            std::cout << gai_strerror(getNameResult) << std::endl;

            WSACleanup();
            return -1;
        }

        std::cout << "Server: Accepted connection from host '" << hostString << "' and port '" << serverString << "'" << std::endl;

        CreateThread(NULL, 0, Server::receiveThread, (LPVOID)instance, 0, NULL);
    }
}

DWORD WINAPI Server::receiveThread(LPVOID param)
{
    Server *instance = static_cast<Server *>(param);

    char receiveBuffer[200];

    while (true)
    {
        int byteCount = recv(instance->acceptSocket, receiveBuffer, 200, 0);
        if (byteCount < 0)
        {
            std::cout << "recv failed: " << std::endl;
            std::cout << WSAGetLastError() << std::endl;
            return -1;
        }

        std::cout << "Server: Received message: " << receiveBuffer << std::endl;

        if (strstr("SHUTDOWN", receiveBuffer))
        {
            instance->shutdown();
            break;
        }

        char sendBuffer[200] = "Message Received";
        send(instance->acceptSocket, sendBuffer, 200, 0);
    }
}

int Server::bindSocket()
{
    if (bind(this->serverSocket, (SOCKADDR *)&this->serverService, sizeof(this->serverService)) == SOCKET_ERROR)
    {
        std::cout << "Server: Binding failed: " << std::endl;
        std::cout << WSAGetLastError() << std::endl;

        closesocket(this->serverSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Server: Binding was successful" << std::endl;

    return 0;
}

int Server::listenSocket()
{
    if (listen(this->serverSocket, 1) == SOCKET_ERROR)
    {
        std::cout << "Server: Error listening on socket: " << std::endl;
        std::cout << WSAGetLastError() << std::endl;

        return -1;
    }

    std::cout << "Server: Now listening. Waiting for connections..." << std::endl;

    return 0;
}

int Server::initialize()
{
    if (Comms::initializeWinsock() != 0)
        return -1;

    if (Comms::createSocket(this->serverSocket) != 0)
        return -1;

    if (Comms::createService(this->serverService) != 0)
        return -1;

    if (bindSocket() != 0)
        return -1;

    if (listenSocket() != 0)
        return -1;

    CreateThread(NULL, 0, Server::acceptThread, (LPVOID)this, 0, NULL);

    return 0;
}

void Server::shutdown()
{
    std::cout << "Server: Shutting down...";
    closesocket(this->serverSocket);
    WSACleanup();
}

#endif