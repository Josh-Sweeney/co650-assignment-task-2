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

            if (strstr("QUIT", buffer))
            {
                std::cout << "Client: Shutting down client..." << std::endl;
                break;
            }

            if (byteCount == SOCKET_ERROR)
            {
                std::string errorMessage = "Client: send() error " + std::to_string(WSAGetLastError());
                throw std::runtime_error(errorMessage);
            }

            std::cout << "Client: send() is OK." << std::endl;

            char responseBuffer[200];

            byteCount = recv(instance->clientSocket, responseBuffer, sizeof(responseBuffer), 0);
            if (byteCount < 0)
            {
                std::string errorMessage = "Client: recv() error " + std::to_string(WSAGetLastError());
                throw std::runtime_error(errorMessage);
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
        std::string errorMessage = "Client: connect() error " + std::to_string(WSAGetLastError());
        throw std::runtime_error(errorMessage);
    }

    std::cout << "Connected to socket." << std::endl;
    std::cout << "Client: Can start sending and receiving data..." << std::endl;
}

void Client::run()
{
    Comms::initializeWinsock();
    Comms::createSocket(this->clientSocket);
    Comms::createService(this->clientService);
    connectSocket();

    // Create a thread to send messages to the server.
    HANDLE sendThreadHandle = CreateThread(NULL, 0, sendThread, this, 0, NULL);

    // Wait for the send thread to finish & then close it.
    WaitForSingleObject(sendThreadHandle, INFINITE);
    CloseHandle(sendThreadHandle);
}

void Client::shutdown()
{
    std::cout << "Client: Shutting down..." << std::endl;
    closesocket(this->clientSocket);
    WSACleanup();
}

#endif