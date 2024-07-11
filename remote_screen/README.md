## Descrição

Este repositório contém dois códigos em C para um sistema de controle remoto de tela. O primeiro código é um cliente que roda em Windows e captura screenshots da tela do cliente, enquanto o segundo é um servidor que roda em Linux e recebe as screenshots do cliente, exibindo-as em uma janela SDL2.

Neste exemplo o código para ser executado no cliente Windows é compilado no Linux, para isso é necessária a instalação de algumas bibliotecas adicionais:

```sudo apt-get install libsdl2-dev -y```
```sudo apt-get install libc6-dev```

- Compilar o código do cliente:


```x86_64-w64-mingw32-gcc -o screen_client.exe screen_client.c -lws2_32 -lgdi32 -lwindowscodecs -lole32```

Servidor (Linux)

- Compilar o código do servidor:

```gcc -o screen_server screen_server.c -lSDL2```


### Cliente (Windows)

- Executar o executável screen_client.exe.

### Servidor (Linux)

- Execute screen_server no Linux.
- Insira os comandos conforme solicitado pelo servidor (SCREEN para capturar a tela do cliente e EXIT para encerrar a conexão).