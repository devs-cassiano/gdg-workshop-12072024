# Roulette

Este projeto contém um programa em C que simula uma "roleta russa" para arquivos, excluindo aleatoriamente um arquivo do diretório onde o executável está localizado. O programa é escrito usando a WinAPI e pode ser compilado no Linux para execução no Windows.

## Funcionalidades

- Lista todos os arquivos no diretório do executável.
- Exclui aleatoriamente um arquivo do diretório.

## Requisitos

- Sistema operacional Linux para compilação.
- MinGW-w64 instalado no Linux para compilar para Windows.
- Sistema operacional Windows para executar o programa compilado.

## Compilação no Linux

```x86_64-w64-mingw32-gcc -o roulette.exe roulette.c```
