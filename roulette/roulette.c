#include <windows.h>  // Biblioteca principal da API do Windows, usada para manipulação de janelas, processos, threads e outras funcionalidades do sistema operacional.
#include <stdio.h>    // Biblioteca padrão de entrada e saída, usada para funções como printf().
#include <stdlib.h>   // Biblioteca padrão de funções utilitárias, como malloc(), free(), rand(), srand(), etc.
#include <time.h>     // Biblioteca para manipulação de data e hora, usada aqui para inicializar a semente do gerador de números aleatórios.

// Função para lidar com erros, imprime a mensagem de erro e finaliza o programa.
void HandleError(const char *message) {
    fprintf(stderr, "%s: %d\n", message, GetLastError());
    exit(1);
}

// Função para listar todos os arquivos em um diretório.
void ListFiles(const char *directory, char files[][MAX_PATH], int *count) {
    WIN32_FIND_DATA findFileData; // Estrutura que armazena informações sobre o arquivo encontrado.
    HANDLE hFind = INVALID_HANDLE_VALUE; // Handle para a pesquisa de arquivos.
    char dirSpec[MAX_PATH]; // Buffer para armazenar o caminho do diretório e o padrão de pesquisa.

    // Cria o padrão de pesquisa no formato "diretório\*.*"
    snprintf(dirSpec, MAX_PATH, "%s\\*.*", directory);
    hFind = FindFirstFile(dirSpec, &findFileData); // Inicia a pesquisa de arquivos.

    if (hFind == INVALID_HANDLE_VALUE) {
        HandleError("FindFirstFile failed"); // Trata o erro se a pesquisa falhar.
    } else {
        do {
            // Ignora diretórios, considerando apenas arquivos.
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                // Armazena o caminho completo do arquivo na matriz files.
                snprintf(files[*count], MAX_PATH, "%s\\%s", directory, findFileData.cFileName);
                (*count)++; // Incrementa o contador de arquivos.
            }
        } while (FindNextFile(hFind, &findFileData) != 0); // Continua a pesquisa pelos próximos arquivos.
        FindClose(hFind); // Fecha o handle da pesquisa.
    }
}

// Função para excluir um arquivo aleatório da lista de arquivos.
void DeleteRandomFile(char files[][MAX_PATH], int count) {
    if (count == 0) {
        printf("No files to delete.\n");
        return;
    }

    srand(time(NULL)); // Inicializa a semente do gerador de números aleatórios.
    int randomIndex = rand() % count; // Gera um índice aleatório na faixa de 0 a count-1.
    if (DeleteFile(files[randomIndex])) { // Tenta excluir o arquivo.
        printf("Deleted file: %s\n", files[randomIndex]); // Mensagem de sucesso.
    } else {
        HandleError("DeleteFile failed"); // Trata o erro se a exclusão falhar.
    }
}

int main() {
    char executablePath[MAX_PATH]; // Buffer para armazenar o caminho do executável.
    char directory[MAX_PATH]; // Buffer para armazenar o caminho do diretório.
    char files[1000][MAX_PATH];  // Matriz para armazenar os caminhos dos arquivos encontrados.
    int count = 0; // Contador de arquivos.

    // Obtém o caminho completo do executável.
    if (GetModuleFileName(NULL, executablePath, MAX_PATH) == 0) {
        HandleError("GetModuleFileName failed"); // Trata o erro se a função falhar.
    }

    // Remove o nome do executável do caminho para obter apenas o diretório.
    char *lastSlash = strrchr(executablePath, '\\');
    if (lastSlash != NULL) {
        *lastSlash = '\0';
    }

    // Copia o caminho do diretório para o buffer directory.
    snprintf(directory, MAX_PATH, "%s", executablePath);

    // Lista todos os arquivos no diretório.
    ListFiles(directory, files, &count);
    // Exclui um arquivo aleatório.
    DeleteRandomFile(files, count);

    return 0;
}
