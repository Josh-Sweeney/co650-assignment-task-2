#ifdef COMPILE_CLIENT

#include "client.h"
#include "comms.h"

DWORD WINAPI Client::sendThread(LPVOID param)
{
    Client *instance = static_cast<Client *>(param);

    char buffer[200];

    while (true)
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
            std::cout << "Client: send() error " << WSAGetLastError() << std::endl;
            instance->shutdown();

            return -1;
        }

        std::cout << "Client: send() is OK." << std::endl;

        char responseBuffer[200];

        byteCount = recv(instance->clientSocket, (char *)&buffer, sizeof(buffer), 0);
        if (byteCount < 0)
        {
            std::cout << "Client: recv() failed: " << std::endl;
            std::cout << WSAGetLastError() << std::endl;

            instance->shutdown();
            return -1;
        }

        std::cout << "Server responded with message: " << responseBuffer << std::endl;
    }
}

int Client::connectSocket()
{
    if (connect(this->clientSocket, (SOCKADDR *)&this->clientService, sizeof(this->clientService)) == SOCKET_ERROR)
    {
        std::cout << "Failed to connect to socket: " << std::endl;

        WSACleanup();
        return -1;
    }

    std::cout << "Connected to socket." << std::endl;
    std::cout << "Client: Can start sending and receiving data..." << std::endl;

    return 0;
}

int Client::initialize()
{
    if (Comms::initializeWinsock() != 0)
        return -1;

    if (Comms::createSocket(this->clientSocket) != 0)
        return -1;

    if (Comms::createService(this->clientService) != 0)
        return -1;

    if (connectSocket() != 0)
        return -1;

    CreateThread(NULL, 0, sendThread, this, 0, NULL);

    return 0;
}

void Client::shutdown()
{
    std::cout << "Client: Shutting down..." << std::endl;
    closesocket(this->clientSocket);
    WSACleanup();
}

#endif