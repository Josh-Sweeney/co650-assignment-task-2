#include "client.h"
#include "comms.h"

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

    sockaddr_in clientService;
    if (Comms::createService(clientService) != 0)
        return -1;

    if (connectSocket() != 0)
        return -1;

    // Exercise 1: Within the client add the code to allow the user to type in a message.
    char buffer[200];

    // Exercise 3: Extend exercise 2 by allowing an unlimited number of messages to be
    // typed into the client and each one should receive a confirmation
    while (true)
    {
        // Exercise 6: Now modify the client so that it no longer sends data to the server
        // but simply receives an instance of Data nad prints the values of the two member
        // variables
        // printf("Type a message to send to server:\n");
        // cin.getline(buffer, 200);
        // int byteCount = send(clientSocket, buffer, 200, 0);

        // Exercise 4: Extend the programs to terminate both the server and client machine
        // if the user types "SHUTDOWN" into the client.
        // NOTE: I placed this code after it sends the message to the server so that the
        // server can receive the message to shutdown so that both the server and client
        // shutdown, not just the client
        // CHALLENGE: I didn't know how to check if a char* contains a string so I did
        // research and found that the strstr() method can be used for this purpose
        if (strstr("SHUTDOWN", buffer))
        {
            cout << "Shutting down client..." << endl;
            break;
        }

        // if (byteCount == SOCKET_ERROR){
        //     cout << "Client: send() error " << WSAGetLastError() << endl;
        //     WSACleanup();
        //     return 0;
        // }
        // else{
        //     cout << "Client: send() is OK." << endl;

        // Exercise 2: Now extend both the client and server so that that on receipt
        // of a message the server sends an automatic confirmation message back to
        // the client ("Message Received"), which is then displayed on the client's screen.
        // char responseBuffer[200];
        Data data;
        int byteCount = recv(clientSocket, (char *)&data, sizeof(Data), 0);
        if (byteCount < 0)
        {
            cout << "recv failed: " << WSAGetLastError() << endl;
            return 0;
        }
        else
        {
            // cout << "Server responded with message: " << responseBuffer << endl;
            printf("Heath: \"%d\"\n", data.health);
            printf("Name: \"%s\"\n", data.name);
        }
        // }
    }

    system("pause");
    WSACleanup();
    return 0;
}