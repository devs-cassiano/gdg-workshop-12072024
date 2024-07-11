Descrição

Este repositório contém dois códigos em C que implementam um simples sistema de Controle Remoto (RAT - Remote Access Tool). O cliente, que deve ser executado em um sistema Windows, pode receber comandos do servidor, que deve ser executado em um sistema Linux. O servidor envia comandos para abrir o Notepad no cliente ou encerrar a conexão.

- Compile o código do cliente com o seguinte comando:

```x86_64-w64-mingw32-gcc -o notepad_client.exe notepad_client.c -lws2_32 -lgdiplus```

- Compilar o código do servidor

```gcc -o notepad_server notepad_server.c```

### Servidor

Comandos para enviar ao cliente após estabelecer comunicações:

- notepad: Envia o comando para o cliente abrir o Notepad.
- exit: Envia o comando para o cliente encerrar a conexão e o servidor termina a execução.

#### Servidor: 

- Execute o servidor no sistema Linux:

```./server```

- O servidor agora está aguardando conexões de clientes.

### Cliente: 

- Execute o cliente no sistema Windows utilizando o Powershell ou CMD

```notepad_client.exe```

#### Comandos: No terminal do servidor, você pode inserir os seguintes comandos:
- notepad: Este comando abrirá o Notepad no sistema cliente.
- exit: Este comando encerrará a conexão com o cliente e terminará a execução do servidor.

