#ifdef COMPILE_CLIENT

#include "client.h"
#include "comms.h"

DWORD WINAPI Client::sendThread(LPVOID param)
{
    Client *instance = static_cast<Client *>(param);

    char buffer[200];

    while (true)
    {
        try
        {
            printf("Type a message to send to server:\n");
            std::cin.getline(buffer, 200);
            int byteCount = send(instance->clientSocket, buffer, 200, 0);

            if (strstr("SHUTDOWN", buffer))
            {
                std::cout << "Client: Shutting down client..." << std::endl;
                break;
            }

            if (byteCount == SOCKET_ERROR)
            {
                throw std::runtime_error("Client: send() error " + WSAGetLastError());
            }

            std::cout << "Client: send() is OK." << std::endl;

            char responseBuffer[200];

            byteCount = recv(instance->clientSocket, (char *)&buffer, sizeof(buffer), 0);
            if (byteCount < 0)
            {
                throw std::runtime_error("Client: recv() error " + WSAGetLastError());
            }

            std::cout << "Server responded with message: " << responseBuffer << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << std::endl;
            instance->shutdown();
        }
    }
}

void Client::connectSocket()
    {
        if (connect(this->clientSocket, (SOCKADDR *)&this->clientService, sizeof(this->clientService)) == SOCKET_ERROR)
        {
            throw std::runtime_error("Client: connect() error " + WSAGetLastError());
        }

        std::cout << "Connected to socket." << std::endl;
        std::cout << "Client: Can start sending and receiving data..." << std::endl;
}

void Client::initialize()
{
    Comms::initializeWinsock();
    Comms::createSocket(this->clientSocket);
    Comms::createService(this->clientService);
    connectSocket();
    CreateThread(NULL, 0, sendThread, this, 0, NULL);
}

void Client::shutdown()
{
    std::cout << "Client: Shutting down..." << std::endl;
    closesocket(this->clientSocket);
    WSACleanup();
}

#endif