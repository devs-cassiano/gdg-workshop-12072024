#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <SDL2/SDL.h>

#define PORT 8080
#define BUFFER_SIZE 4096
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

void HandleError(const char *message) {
    perror(message);
    exit(1);
}

void SaveScreenshot(int clientSocket, SDL_Renderer *renderer, SDL_Texture *texture) {
    uint32_t screenshotSize;
    recv(clientSocket, &screenshotSize, sizeof(screenshotSize), 0);

    unsigned char *buffer = (unsigned char *)malloc(screenshotSize);
    if (!buffer) {
        HandleError("Memory allocation failed");
    }

    int receivedBytes = 0;
    while (receivedBytes < screenshotSize) {
        int bytes = recv(clientSocket, buffer + receivedBytes, screenshotSize - receivedBytes, 0);
        if (bytes <= 0) {
            HandleError("Failed to receive screenshot data");
        }
        receivedBytes += bytes;
    }

    SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(buffer, SCREEN_WIDTH, SCREEN_HEIGHT, 32, SCREEN_WIDTH * 4,
                                                    0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    if (surface == NULL) {
        free(buffer);
        HandleError("SDL_CreateRGBSurfaceFrom failed");
    }

    SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_FreeSurface(surface);
    free(buffer);
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];

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

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        HandleError("SDL_Init failed");
    }

    SDL_Window *window = SDL_CreateWindow("Remote Screen",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (window == NULL) {
        SDL_Quit();
        HandleError("SDL_CreateWindow failed");
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        HandleError("SDL_CreateRenderer failed");
    }

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (texture == NULL) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        HandleError("SDL_CreateTexture failed");
    }

    while (1) {
        printf("Insira um um comando:\n> SCREEN (Print da tela)\n> EXIT (Sair)\nComando:  ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0; // Remove the newline character

        if (send(clientSocket, buffer, strlen(buffer), 0) == -1) {
            HandleError("Send failed");
        }

        if (strncmp(buffer, "SCREEN", 6) == 0) {
            SaveScreenshot(clientSocket, renderer, texture);
        } else if (strncmp(buffer, "EXIT", 4) == 0) {
            printf("Exit command sent\n");
            break;
        }
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    close(clientSocket);
    close(serverSocket);

    return 0;
}
