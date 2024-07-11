#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define SERVER_IP "192.168.0.115"
#define BUFFER_SIZE 1024

void HandleError(const char *message) {
    fprintf(stderr, "%s: %d\n", message, WSAGetLastError());
    exit(1);
}

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE] = {0};

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        HandleError("WSAStartup failed");
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        HandleError("Socket creation failed");
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(PORT);

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        HandleError("Connect failed");
    }

    printf("Connected to server\n");

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        if (recv(clientSocket, buffer, BUFFER_SIZE, 0) == SOCKET_ERROR) {
            HandleError("Receive failed");
        }

        printf("Command received: %s\n", buffer);

        if (strcmp(buffer, "notepad") == 0) {
            system("notepad.exe");
        } else if (strcmp(buffer, "exit") == 0) {
            break;
        }
    }

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
