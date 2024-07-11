#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void HandleError(const char *message) {
    perror(message);
    exit(1);
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    char buffer[BUFFER_SIZE] = {0};

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        HandleError("Socket creation failed");
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        HandleError("Bind failed");
    }

    if (listen(serverSocket, 3) == -1) {
        HandleError("Listen failed");
    }

    printf("Waiting for incoming connections...\n");
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen);
    if (clientSocket == -1) {
        HandleError("Accept failed");
    }

    printf("Connection accepted\n");

    while (1) {
        printf("Enter command: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0; // Remove the newline character

        if (send(clientSocket, buffer, strlen(buffer), 0) == -1) {
            HandleError("Send failed");
        }

        if (strcmp(buffer, "exit") == 0) {
            break;
        }
    }

    close(clientSocket);
    close(serverSocket);

    return 0;
}
