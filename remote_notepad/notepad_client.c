#include <stdio.h> // Biblioteca padrão de entrada e saída
#include <stdlib.h> // Biblioteca padrão de funções utilitárias
#include <string.h> // Biblioteca para manipulação de strings
#include <winsock2.h> // Biblioteca para programação de sockets no Windows
#include <windows.h> // Biblioteca principal da API do Windows

#pragma comment(lib, "ws2_32.lib") // Inclui a biblioteca ws2_32.lib (sockets) no link

#define PORT 8080 // Define a porta do servidor
#define SERVER_IP "192.168.0.115" // Define o IP do servidor
#define BUFFER_SIZE 1024 // Define o tamanho do buffer de comunicação

// Função para lidar com erros, imprime a mensagem de erro e finaliza o programa
void HandleError(const char *message) {
    fprintf(stderr, "%s: %d\n", message, WSAGetLastError()); // Imprime a mensagem de erro e o código do erro do Windows Sockets
    exit(1); // Encerra o programa com status de erro
}

int main() {
    WSADATA wsaData; // Estrutura para dados do Windows Sockets
    SOCKET clientSocket; // Socket do cliente
    struct sockaddr_in serverAddr; // Estrutura para endereço do servidor
    char buffer[BUFFER_SIZE] = {0}; // Buffer para comunicação

    // Inicializa o uso do Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        HandleError("WSAStartup failed");
    }

    // Cria um socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        HandleError("Socket creation failed");
    }

    // Configura os parâmetros do servidor
    serverAddr.sin_family = AF_INET; // Define a família de endereços como AF_INET (IPv4)
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP); // Define o endereço IP do servidor
    serverAddr.sin_port = htons(PORT); // Converte a porta para o formato de rede

    // Conecta ao servidor
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        HandleError("Connect failed");
    }

    printf("Conectado ao servidor\n");

    // Loop principal para receber comandos do servidor
    while (1) {
        memset(buffer, 0, BUFFER_SIZE); // Limpa o buffer
        // Recebe dados do servidor
        if (recv(clientSocket, buffer, BUFFER_SIZE, 0) == SOCKET_ERROR) {
            HandleError("Receive failed");
        }

        printf("Comando recebido: %s\n", buffer);

        // Se o comando for "notepad", abre o Notepad
        if (strcmp(buffer, "notepad") == 0) {
            system("notepad.exe");
        } 
        // Se o comando for "exit", encerra a conexão
        else if (strcmp(buffer, "exit") == 0) {
            break;
        }
    }

    // Fecha o socket e limpa os recursos do Winsock
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
