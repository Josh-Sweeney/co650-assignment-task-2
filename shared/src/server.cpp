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
        try
        {
            instance->acceptSocket = accept(instance->serverSocket, (SOCKADDR *)&from, &fromLength);
            if (instance->acceptSocket == INVALID_SOCKET)
            {
                throw std::runtime_error("Server: Accept failed: " + WSAGetLastError());
            }

            int getNameResult = getnameinfo((SOCKADDR *)&from, fromLength, hostString, NI_MAXHOST, serverString, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

            if (getNameResult != 0)
            {
                throw std::runtime_error("Server: Failed to get name info: " + std::string(gai_strerror(getNameResult)));
            }

            std::cout << "Server: Accepted connection from host '" << hostString << "' and port '" << serverString << "'" << std::endl;

            CreateThread(NULL, 0, Server::receiveThread, (LPVOID)instance, 0, NULL);
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << std::endl;
            instance->shutdown();
        }
    }
}

DWORD WINAPI Server::receiveThread(LPVOID param)
{
    Server *instance = static_cast<Server *>(param);

    char receiveBuffer[200];

    while (true)
    {
        try
        {
            int byteCount = recv(instance->acceptSocket, receiveBuffer, 200, 0);
            if (byteCount < 0)
            {
                throw std::runtime_error("Server: recv() error " + WSAGetLastError());
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
        catch (const std::exception &e)
        {
            std::cout << e.what() << std::endl;
            instance->shutdown();
        }
    }
}

void Server::bindSocket()
{
    try
    {
        if (bind(this->serverSocket, (SOCKADDR *)&this->serverService, sizeof(this->serverService)) == SOCKET_ERROR)
        {
            throw std::runtime_error("Server: Binding failed: " + WSAGetLastError());
        }

        std::cout << "Server: Binding was successful" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
        closesocket(this->serverSocket);
        WSACleanup();
    }
}

void Server::listenSocket()
{
    try
    {
        if (listen(this->serverSocket, 1) == SOCKET_ERROR)
        {
            throw std::runtime_error("Server: Listening failed: " + WSAGetLastError());
        }

        std::cout << "Server: Now listening. Waiting for connections..." << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
        closesocket(this->serverSocket);
        WSACleanup();
    }
}

void Server::initialize()
{
    Comms::initializeWinsock();
    Comms::createSocket(this->serverSocket);
    Comms::createService(this->serverService);
    bindSocket();
    listenSocket();

    CreateThread(NULL, 0, Server::acceptThread, (LPVOID)this, 0, NULL);
}

void Server::shutdown()
{
    std::cout << "Server: Shutting down...";
    closesocket(this->serverSocket);
    WSACleanup();
}

#endif