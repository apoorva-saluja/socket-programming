#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

using namespace std;

int main() {
    WSADATA wsaData;
    int wsaerr = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaerr != 0) {
        cerr << "WSAStartup failed with error: " << wsaerr << endl;
        return 1;
    }
    cout << "WSAStartup successful" << endl;

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Socket creation failed with error: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }
    cout << "Socket created successfully" << endl;

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // Replace "127.0.0.1" with the server IP if needed

    if (serverAddress.sin_addr.s_addr == INADDR_NONE) {
        cerr << "Invalid address/ Address not supported" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        cerr << "Connect failed with error: " << WSAGetLastError() << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    cout << "Connected to server" << endl;

    const char* message = "Hello, server!";
    int sendResult = send(clientSocket, message, (int)strlen(message), 0);
    if (sendResult == SOCKET_ERROR) {
        cerr << "Send failed with error: " << WSAGetLastError() << endl;
    } else {
        cout << "Message sent to server" << endl;
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
