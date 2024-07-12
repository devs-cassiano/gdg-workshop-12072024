#include <windows.h> // Biblioteca principal da API do Windows, usada para manipulação de janelas, processos, threads e outras funcionalidades do sistema operacional.
#include <tlhelp32.h> // Biblioteca para criar snapshots de processos/snapshots. Utilizada para obter informações sobre os processos e threads do sistema.
#include <stdio.h> // Biblioteca padrão de entrada e saída, usada para funções como printf().


// Função para obter o ID do processo pelo nome
DWORD GetProcessIdByName(const char* processName) {
    PROCESSENTRY32 processEntry; // Estrutura que armazena informações sobre um processo.
    processEntry.dwSize = sizeof(PROCESSENTRY32); // Define o tamanho da estrutura PROCESSENTRY32.

    // Cria um snapshot de todos os processos do sistema.
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return 0; // Retorna 0 se a criação do snapshot falhar.
    }

    // Obtém o primeiro processo do snapshot.
    if (Process32First(snapshot, &processEntry)) {
        do {
            // Compara o nome do processo atual com o nome fornecido.
            if (strcmp(processEntry.szExeFile, processName) == 0) {
                // Se o processo for encontrado, fecha o snapshot e retorna o ID do processo.
                CloseHandle(snapshot);
                return processEntry.th32ProcessID;
            }
        } while (Process32Next(snapshot, &processEntry)); // Itera sobre os processos no snapshot.
    }

    // Fecha o snapshot e retorna 0 se o processo não for encontrado.
    CloseHandle(snapshot);
    return 0;
}

// Função para injetar uma DLL em um processo.
void InjectDLL(DWORD processId, const char* dllPath) {
    // Abre o processo com permissões totais.
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (hProcess == NULL) {
        printf("Failed to open target process.\n");
        return;
    }

    // Aloca memória no processo alvo para armazenar o caminho da DLL.
    LPVOID pDllPath = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
    if (pDllPath == NULL) {
        printf("Failed to allocate memory in target process.\n");
        CloseHandle(hProcess);
        return;
    }

    // Escreve o caminho da DLL na memória alocada do processo alvo.
    if (!WriteProcessMemory(hProcess, pDllPath, (LPVOID)dllPath, strlen(dllPath) + 1, NULL)) {
        printf("Failed to write DLL path to target process memory.\n");
        VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return;
    }

    // Obtém um handle para o módulo Kernel32, que contém a função LoadLibraryA.
    HMODULE hKernel32 = GetModuleHandle("Kernel32");
    if (hKernel32 == NULL) {
        printf("Failed to get handle for Kernel32.\n");
        VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return;
    }

    // Obtém o endereço da função LoadLibraryA do Kernel32.
    LPTHREAD_START_ROUTINE pLoadLibrary = (LPTHREAD_START_ROUTINE)GetProcAddress(hKernel32, "LoadLibraryA");
    if (pLoadLibrary == NULL) {
        printf("Failed to get address for LoadLibraryA.\n");
        VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return;
    }

    // Cria um thread remoto no processo alvo para executar LoadLibraryA com o caminho da DLL.
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, pLoadLibrary, pDllPath, 0, NULL);
    if (hThread == NULL) {
        printf("Failed to create remote thread in target process.\n");
        VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return;
    }

    // Aguarda a conclusão da execução do thread remoto.
    WaitForSingleObject(hThread, INFINITE);

    // Libera a memória alocada e fecha os handles.
    VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);
}

// Função para esperar o MS Paint ser aberto e injetar a DLL.
void WaitForMSPaintAndInject(const char* dllPath) {
    printf("Aguardando MS Paint ser aberto...\n");
    while (1) {
        // Obtém o ID do processo do MS Paint.
        DWORD processId = GetProcessIdByName("mspaint.exe");
        if (processId != 0) {
            printf("Processo MS Paint encontrado: PID %lu\n", processId);
            InjectDLL(processId, dllPath); // Injeta a DLL no processo do MS Paint.
            printf("DLL injetada com sucesso!\n");
            break; // Sai do loop após a injeção.
        }
        Sleep(1000); // Espera por 1 segundo antes de verificar novamente.
    }
}

// Função principal.
int main() {
    const char* dllPath = "C:\\Users\\user\\Documents\\injection\\injected.dll"; // Caminho da DLL a ser injetada.
    WaitForMSPaintAndInject(dllPath); // Chama a função para esperar o MS Paint e injetar a DLL.
    return 0;
}
