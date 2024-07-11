#include <windows.h>

void DisplayMessage() {
    MessageBox(NULL, "Deu ruim brother. Tu foi hackeado.", "Mensagem", MB_OK | MB_ICONEXCLAMATION);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            DisplayMessage();
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}
