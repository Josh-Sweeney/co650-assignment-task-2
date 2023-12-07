#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    SOCKET clientSocket;
    int port = 55555;
    WSAData wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);
    if (wsaerr != 0){
        cout << "The Winsock dll not found!" << endl;
        return 0;
    }
    else{
        cout << "The Winsock dll found!" << endl;
        cout << "The status: " << wsaData.szSystemStatus << endl;
    }

    clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET){
        cout << "Error at socket(): " << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }
    else{
        cout << "socket() is OK!" << endl;
    }

    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    InetPton(AF_INET, _T("127.0.0.1"), &clientService.sin_addr.s_addr);
    clientService.sin_port = htons(port);
    if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR){
        cout << "Client: connect() - Failed to connect. " << endl;
        WSACleanup();
        return 0;
    }
    else{
        cout << "Client: connect() is OK." << endl;
        cout << "Client: Can start sending and receiving data..." << endl;
    }
    system("pause");
    WSACleanup();
    return 0;
}