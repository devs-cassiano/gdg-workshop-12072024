# Workshop GDG Sec Day 2024 - Explorando o Mundo do Desenvolvimento de Malwares

Este repositório contém dois exemplos de códigos de Remote Access Tool (RAT) escritos em C. 
- O primeiro exemplo permite a abertura remota do Notepad em um computador Windows.
- O segundo exemplo obtém capturas de tela da tela de um computador Windows e as envia para um servidor Linux. 
- O terceiro exemplo simula a injeção de DLL em um processo no Windows

Obs: Todos os códigos foram compilados no S.O. Linux ParrotOS.

## Abrir o Notepad Remotamente

### Descrição

Este código exemplifica um RAT permite ao servidor enviar um comando ao cliente para abrir o Notepad remotamente em um computador Windows. O cliente recebe o comando e executa a ação de abrir o Notepad.

- Link para o código: [remote_notepad](remote_notepad)

#### Servidor (Linux)
- O servidor aguarda conexões de clientes e envia o comando para abrir o Notepad.
#### Cliente (Windows)
- O cliente se conecta ao servidor, aguarda comandos e, ao receber o comando específico, abre o Notepad.

## RAT para screenshots

### Descrição

Este código de exemplo de um RAT permite ao servidor enviar um comando ao cliente para capturar a tela de um computador Windows. As capturas de tela são então enviadas do cliente para o servidor, onde podem ser visualizadas.

- Link para o código: [remote_screen](remote_screen)

#### Servidor (Linux)
- O servidor aguarda conexões de clientes e envia o comando para capturar a tela.
- Recebe a captura de tela enviada pelo cliente e a exibe 
#### Cliente (Windows)
- O cliente se conecta ao servidor, aguarda comandos e, ao receber o comando específico, captura a tela e envia a imagem para o servidor.

## DLL Process Injection

Este código é um exemplo de injeção de DLL em um processo do mspaint no Windows.

- Link para o código: [dll_process_injection](dll_process_injection)

#### Funcionamento
- O executável injetor da dll aguarda a abertura do processo do mspaint e exibe uma janela com um aviso.
