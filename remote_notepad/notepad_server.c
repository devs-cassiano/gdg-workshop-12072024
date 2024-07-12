#include <stdio.h> // Biblioteca padrão de entrada e saída
#include <stdlib.h> // Biblioteca padrão de funções utilitárias
#include <string.h> // Biblioteca para manipulação de strings
#include <unistd.h> // Biblioteca para chamadas de sistema POSIX (como close)
#include <arpa/inet.h> // Biblioteca para manipulação de endereços de internet
#include <sys/socket.h> // Biblioteca para programação de sockets

#define PORT 8080 // Define a porta do servidor
#define BUFFER_SIZE 1024 // Define o tamanho do buffer de comunicação

// Função para lidar com erros
void HandleError(const char *message) {
    perror(message); // Imprime a mensagem de erro fornecida e o erro do sistema
    exit(1); // Encerra o programa com status de erro
}

// Função principal
int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    char buffer[BUFFER_SIZE] = {0};

    // Cria um socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        HandleError("Socket creation failed");
    }

    // Configura os parâmetros do servidor
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Aceita conexões de qualquer endereço IP
    serverAddr.sin_port = htons(PORT); // Converte a porta para formato de rede

    // Associa o socket ao endereço e porta especificados
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        HandleError("Bind failed");
    }

    // Coloca o socket em modo de escuta, permitindo aceitar conexões
    if (listen(serverSocket, 3) == -1) {
        HandleError("Listen failed");
    }

    printf("Aguardando conexao do cliente...\n");
    // Aceita uma conexão do cliente
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen);
    if (clientSocket == -1) {
        HandleError("Accept failed");
    }

    printf("Cliente conectado\n");

    // Loop principal para receber comandos do usuário
    while (1) {
        printf("Insira o comando 'notepad' para abrir o Notepad ou 'exit' para sair: ");
        fgets(buffer, BUFFER_SIZE, stdin); // Lê o comando do usuário
        buffer[strcspn(buffer, "\n")] = 0; // Remove o caractere de nova linha

        // Envia o comando para o cliente
        if (send(clientSocket, buffer, strlen(buffer), 0) == -1) {
            HandleError("Send failed");
        }

        // Se o comando for "exit", encerra o loop
        if (strcmp(buffer, "exit") == 0) {
            break;
        }
    }

    // Fecha os sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}
