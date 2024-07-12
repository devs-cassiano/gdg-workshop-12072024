#include <stdio.h> // Biblioteca padrão de entrada e saída
#include <stdlib.h> // Biblioteca padrão de funções utilitárias
#include <string.h> // Biblioteca para manipulação de strings
#include <unistd.h> // Biblioteca para chamadas de sistema POSIX (como close)
#include <arpa/inet.h> // Biblioteca para manipulação de endereços de internet
#include <sys/socket.h> // Biblioteca para programação de sockets
#include <SDL2/SDL.h> // Biblioteca SDL2 para gráficos e manipulação de janelas

#define PORT 8080 // Define a porta do servidor
#define BUFFER_SIZE 4096 // Define o tamanho do buffer de comunicação
#define SCREEN_WIDTH 1920 // Define a largura da tela
#define SCREEN_HEIGHT 1080 // Define a altura da tela

// Função para lidar com erros
void HandleError(const char *message) {
    perror(message); // Imprime a mensagem de erro fornecida e o erro do sistema
    exit(1); // Encerra o programa com status de erro
}

// Função para salvar uma captura de tela recebida do cliente
void SaveScreenshot(int clientSocket, SDL_Renderer *renderer, SDL_Texture *texture) {
    uint32_t screenshotSize;
    // Recebe o tamanho da captura de tela do cliente
    recv(clientSocket, &screenshotSize, sizeof(screenshotSize), 0);

    // Aloca memória para armazenar a captura de tela
    unsigned char *buffer = (unsigned char *)malloc(screenshotSize);
    if (!buffer) {
        HandleError("Memory allocation failed");
    }

    // Recebe os dados da captura de tela em partes
    int receivedBytes = 0;
    while (receivedBytes < screenshotSize) {
        int bytes = recv(clientSocket, buffer + receivedBytes, screenshotSize - receivedBytes, 0);
        if (bytes <= 0) {
            HandleError("Failed to receive screenshot data");
        }
        receivedBytes += bytes;
    }

    // Cria uma superfície SDL a partir dos dados da captura de tela
    SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(buffer, SCREEN_WIDTH, SCREEN_HEIGHT, 32, SCREEN_WIDTH * 4,
                                                    0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    if (surface == NULL) {
        free(buffer);
        HandleError("SDL_CreateRGBSurfaceFrom failed");
    }

    // Atualiza a textura SDL com a superfície criada
    SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch);
    SDL_RenderClear(renderer); // Limpa o renderizador
    SDL_RenderCopy(renderer, texture, NULL, NULL); // Copia a textura para o renderizador
    SDL_RenderPresent(renderer); // Atualiza a tela com o renderizador

    SDL_FreeSurface(surface); // Libera a superfície
    free(buffer); // Libera o buffer
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];

    // Cria um socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        HandleError("Socket creation failed");
    }

    // Configura os parâmetros do servidor
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

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

    // Inicializa a biblioteca SDL2
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        HandleError("SDL_Init failed");
    }

    // Cria uma janela SDL
    SDL_Window *window = SDL_CreateWindow("Remote Screen",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (window == NULL) {
        SDL_Quit();
        HandleError("SDL_CreateWindow failed");
    }

    // Cria um renderizador SDL para a janela
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        HandleError("SDL_CreateRenderer failed");
    }

    // Cria uma textura SDL para a renderização
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (texture == NULL) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        HandleError("SDL_CreateTexture failed");
    }

    // Loop principal para receber comandos do usuário
    while (1) {
        printf("Insira um um comando:\n> SCREEN (Print da tela)\n> EXIT (Sair)\nComando:  ");
        fgets(buffer, BUFFER_SIZE, stdin); // Lê o comando do usuário
        buffer[strcspn(buffer, "\n")] = 0; // Remove o caractere de nova linha

        // Envia o comando para o cliente
        if (send(clientSocket, buffer, strlen(buffer), 0) == -1) {
            HandleError("Send failed");
        }

        // Se o comando for "SCREEN", salva a captura de tela recebida do cliente
        if (strncmp(buffer, "SCREEN", 6) == 0) {
            SaveScreenshot(clientSocket, renderer, texture);
        } 
        // Se o comando for "EXIT", encerra o loop
        else if (strncmp(buffer, "EXIT", 4) == 0) {
            printf("Exit command sent\n");
            break;
        }
    }

    // Libera os recursos da SDL
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    // Fecha os sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}
