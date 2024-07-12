#include <windows.h> // Biblioteca principal da API do Windows

// Função para exibir uma mensagem de alerta ao usuário
void DisplayMessage() {
    // Cria uma MessageBox com uma mensagem de alerta, um título, e um ícone de exclamação
    MessageBox(NULL, "Deu ruim brother. Tu foi hackeado.", "Mensagem", MB_OK | MB_ICONEXCLAMATION);
}

// Função de entrada principal para a DLL. É chamada automaticamente pelo sistema quando a DLL é carregada ou descarregada.
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
    // Verifica o motivo pelo qual a função DllMain foi chamada
    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            // Quando um processo carrega a DLL, chama a função DisplayMessage
            DisplayMessage();
            break;
        case DLL_THREAD_ATTACH:
            // Quando uma nova thread é criada no processo que já está usando a DLL
            // Neste exemplo, não fazemos nada específico para esta situação
            break;
        case DLL_THREAD_DETACH:
            // Quando uma thread está prestes a ser destruída
            // Neste exemplo, não fazemos nada específico para esta situação
            break;
        case DLL_PROCESS_DETACH:
            // Quando um processo está descarregando a DLL
            // Neste exemplo, não fazemos nada específico para esta situação
            break;
    }
    // Retorna TRUE para indicar que a DLL foi inicializada corretamente
    return TRUE;
}
