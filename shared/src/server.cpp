#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>
#include <iostream>

#include "Comms.h"

DWORD WINAPI AcceptThread(LPVOID param) {
    // param is the accept socket
    SOCKET acceptSocket = (SOCKET)param;

    // Exercise 1: Add code to the server to capture this transmitted message and display
    // it on the screen.
    char receiveBuffer[200];

    // Exercise 3: Extend exercise 2 by allowing an unlimited number of messages to be
    // typed into the client and each one should receive a confirmation
    while(true) {
        int byteCount = recv(acceptSocket, receiveBuffer, 200, 0);
        if (byteCount < 0) {
            cout << "recv failed: " << WSAGetLastError() << endl;
            return 0;
        } else{
            cout << "Received message: " << receiveBuffer << endl;

            // Exercise 4: Extend the programs to terminate both the server and client machine
            // if the user types "SHUTDOWN" into the client.
            if (strstr("SHUTDOWN", receiveBuffer)) {
                cout << "Shutting down server..." << endl;
                break;
            }

            // Exercise 2: Now extend both the client and server so that that on receipt
            // of a message the server sends an automatic confirmation message back to
            // the client ("Message Received"), which is then displayed on the client's screen.
            char sendBuffer[200] = "Message Received";
            send(acceptSocket, sendBuffer, 200, 0);
        }
    }
}

// Exercise 6: WIthin server create a new Thread Function, that declares a static instance
// of Data and assigns it an initial health and name value. Then within a loop increments
// the health by one and transmits the data object to the client every second
DWORD WINAPI DataThread(LPVOID param) {
    static Data data;
    data.health = 100;
    strcpy(data.name, "John");

    while (true) {
        send((SOCKET)param, (char*)&data, sizeof(Data), 0);
        data.health++;
        Sleep(1000);
    }
}

int main(int argc, char *argv[])
{
    SOCKADDR_STORAGE from;
    int retval, fromlen;
    char servstr[NI_MAXSERV], hoststr[NI_MAXHOST];

    SOCKET serverSocket, acceptSocket;

    if (Comms::initializeWinsock() != 0)
        return 0;
    
    if (Comms::createSocket(serverSocket) != 0)
        return 0;

    sockaddr_in service;
    service.sin_family = AF_INET;
    InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr);
    service.sin_port = htons(port);
    if (bind(serverSocket, (SOCKADDR *)&service, sizeof(service)) == SOCKET_ERROR){
        cout << "bind() failed: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }
    else{
        cout << "bind() is OK!" << endl;
    }

    if (listen(serverSocket, 1) == SOCKET_ERROR)
        cout << "listen(): Error listening on socket " << WSAGetLastError() << endl;
    else
        cout << "listen() is OK, I'm waiting for connections..." << endl;

    fromlen = sizeof(from);

    while (true) {
        acceptSocket = accept(serverSocket, (SOCKADDR*)&from, &fromlen);
        if (acceptSocket == INVALID_SOCKET){
            cout << "accept failed: " << WSAGetLastError() << endl;
            WSACleanup();
            return -1;
        }

        retval = getnameinfo(
            (SOCKADDR *)&from,
            fromlen,
            hoststr,
            NI_MAXHOST,
            servstr,
            NI_MAXSERV,
            NI_NUMERICHOST | NI_NUMERICSERV);

        if (retval != 0) {
            cout << "getnameinfo failed: " << retval << endl;
            WSACleanup();
            return -1;
        }

        cout << "Accepted connection from host " << hoststr << " and port " << servstr << endl;

        // Exercise 6: Within main comment out the statement that was created in the previous
        // exercises, and the thread, and add a new one that creates your new thread function
        // CreateThread(NULL, 0, AcceptThread, (LPVOID)acceptSocket, 0, NULL);
        CreateThread(NULL, 0, DataThread, (LPVOID)acceptSocket, 0, NULL);
    }

    system("pause");
    WSACleanup();

    return 0;
}