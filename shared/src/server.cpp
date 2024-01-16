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
            if (instance->shutdownRequested)
            {
                break;
            }

            instance->acceptSocket = accept(instance->serverSocket, (SOCKADDR *)&from, &fromLength);
            if (instance->acceptSocket == INVALID_SOCKET)
            {
                std::string errorMessage = "Server: Accept failed: " + std::to_string(WSAGetLastError());
                throw std::runtime_error(errorMessage);
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
    char sendBuffer[200];

    while (true)
    {
        try
        {
            if (instance->shutdownRequested)
            {
                break;
            }

            int byteCount = recv(instance->acceptSocket, receiveBuffer, 200, 0);
            if (byteCount < 0)
            {
                std::string errorMessage = "Server: recv() error " + std::to_string(WSAGetLastError());
                throw std::runtime_error(errorMessage);
            }

            std::cout << "Server: Received message: " << receiveBuffer << std::endl;
            
            if (strstr("QUIT", receiveBuffer))
            {
                instance->shutdown();
                break;
            }

            printf("Type a message to send to Client:\n");
            std::cin.getline(sendBuffer, 200);
            byteCount = send(instance->acceptSocket, sendBuffer, 200, 0);

            if (byteCount == SOCKET_ERROR)
            {
                std::string errorMessage = "Server: send() error " + std::to_string(WSAGetLastError());
                throw std::runtime_error(errorMessage);
            }

            std::cout << "Server: send() is OK." << std::endl;

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
    if (bind(this->serverSocket, (SOCKADDR *)&this->serverService, sizeof(this->serverService)) == SOCKET_ERROR)
    {
        std::string errorMessage = "Server: Binding failed: " + std::to_string(WSAGetLastError());
        throw std::runtime_error(errorMessage);
    }

    std::cout << "Server: Binding was successful" << std::endl;
}

void Server::listenSocket()
{
    if (listen(this->serverSocket, 1) == SOCKET_ERROR)
    {
        std::string errorMessage = "Server: Listening failed: " + std::to_string(WSAGetLastError());
        throw std::runtime_error(errorMessage);
    }

    std::cout << "Server: Now listening. Waiting for connections..." << std::endl;
}

void Server::run()
{
    Comms::initializeWinsock();
    Comms::createSocket(this->serverSocket);
    Comms::createService(this->serverService);
    bindSocket();
    listenSocket();

    // Create a thread to accept connections.
    // This thread will then create a new thread for each connection.
    HANDLE acceptThread = CreateThread(NULL, 0, Server::acceptThread, (LPVOID)this, 0, NULL);

    // Wait for the accept thread to finish & then close it.
    WaitForSingleObject(acceptThread, INFINITE);
    CloseHandle(acceptThread);
}

void Server::shutdown()
{
    if (!this->shutdownRequested)
    {
        std::cout << "Server: Shutting down..." << std::endl;
    }

    this->shutdownRequested = true;
    closesocket(this->serverSocket);
    WSACleanup();
}

#endif