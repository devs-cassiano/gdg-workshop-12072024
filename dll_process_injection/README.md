# Injeção de DLL no MS Paint

Este exemplo contém dois programas em C que demonstram como injetar uma DLL em um processo do MS Paint no Windows. O primeiro programa é responsável por localizar o processo do MS Paint e injetar a DLL, enquanto o segundo programa é a própria DLL que exibe uma mensagem de alerta quando injetada.

## Funcionalidades

- Localiza o processo do MS Paint.
- Injeta uma DLL no processo do MS Paint.
- Exibe uma mensagem de alerta ao usuário quando a DLL é carregada.

## Compilação no Linux

- Compilar a dll
```x86_64-w64-mingw32-gcc -shared -o injected.dll injected.c -Wl,--subsystem,windows```
- Compilar o injetor
```x86_64-w64-mingw32-gcc -o injector.exe injector.c -luser32 -lkernel32```

## Execução

- Executar o injector.exe
- Abrir o mspaint