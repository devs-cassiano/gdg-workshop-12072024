# Workshop GDG - Explorando o Mundo do Desenvolvimento de Malwares

Este repositório contém dois exemplos de códigos de Remote Access Tool (RAT) escritos em C. O primeiro código permite a abertura remota do Notepad em um computador Windows, enquanto o segundo código obtém capturas de tela da tela de um computador Windows e as envia para um servidor Linux.

## Abrir o Notepad Remotamente

### Descrição

Este código exemplifica um RAT permite ao servidor enviar um comando ao cliente para abrir o Notepad remotamente em um computador Windows. O cliente recebe o comando e executa a ação de abrir o Notepad.
Estrutura do Código

#### Servidor (Linux)
- O servidor aguarda conexões de clientes e envia o comando para abrir o Notepad.
#### Cliente (Windows)
- O cliente se conecta ao servidor, aguarda comandos e, ao receber o comando específico, abre o Notepad.

## RAT para screenshots

### Descrição

Este código de exemplo de um RAT permite ao servidor enviar um comando ao cliente para capturar a tela de um computador Windows. As capturas de tela são então enviadas do cliente para o servidor, onde podem ser visualizadas.

#### Servidor (Linux)
- O servidor aguarda conexões de clientes e envia o comando para capturar a tela.
- Recebe a captura de tela enviada pelo cliente e a exibe 
#### Cliente (Windows)
- O cliente se conecta ao servidor, aguarda comandos e, ao receber o comando específico, captura a tela e envia a imagem para o servidor.