#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

void HandleError(const char* message) {
    fprintf(stderr, "%s: %d\n", message, GetLastError());
    exit(1);
}

DWORD GetProcessIdByName(const char* processName) {
    PROCESSENTRY32 pe32;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        HandleError("CreateToolhelp32Snapshot failed");
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hSnapshot, &pe32)) {
        CloseHandle(hSnapshot);
        HandleError("Process32First failed");
    }

    do {
        if (strcmp(pe32.szExeFile, processName) == 0) {
            CloseHandle(hSnapshot);
            return pe32.th32ProcessID;
        }
    } while (Process32Next(hSnapshot, &pe32));

    CloseHandle(hSnapshot);
    return 0;
}

void InjectDLL(DWORD processId, const char* dllPath) {
    HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, processId);
    if (hProcess == NULL) {
        HandleError("OpenProcess failed");
    }

    LPVOID pRemoteMemory = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
    if (pRemoteMemory == NULL) {
        HandleError("VirtualAllocEx failed");
    }

    if (!WriteProcessMemory(hProcess, pRemoteMemory, dllPath, strlen(dllPath) + 1, NULL)) {
        HandleError("WriteProcessMemory failed");
    }

    LPVOID pLoadLibrary = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
    if (pLoadLibrary == NULL) {
        HandleError("GetProcAddress failed");
    }

    HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibrary, pRemoteMemory, 0, NULL);
    if (hRemoteThread == NULL) {
        HandleError("CreateRemoteThread failed");
    }

    WaitForSingleObject(hRemoteThread, INFINITE);

    VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
    CloseHandle(hRemoteThread);
    CloseHandle(hProcess);
}

void WaitForMSPaintAndInject(const char* dllPath) {
    printf("Aguardando MS Paint ser aberto...\n");
    while (1) {
        DWORD processId = GetProcessIdByName("mspaint.exe");
        if (processId != 0) {
            printf("Processo MS Paint encontrado: PID %lu\n", processId);
            InjectDLL(processId, dllPath);
            printf("DLL injetada com sucesso!\n");
            break;
        }
        Sleep(1000);  // Espera por 1 segundo antes de verificar novamente
    }
}

int main() {
    const char* dllPath = "C:\\Users\\user\\Documents\\injection\\injected.dll";  // Caminho relativo para a DLL

    // Verifica se a DLL existe no diretório atual
    if (GetFileAttributes(dllPath) == INVALID_FILE_ATTRIBUTES) {
        fprintf(stderr, "DLL não encontrada: %s\n", dllPath);
        return 1;
    }

    WaitForMSPaintAndInject(dllPath);
    return 0;
}
