# Algoritmos de alocação de memória

## Sobre

Engenharia de Computação, UEPG - 5º período

Projeto realizado para a disciplina de Sistemas Operacionais

Marcos Renan Krul

## Algoritmos de alocação implementados

1. First fit
2. Next fit
3. Best fit
4. Worst fit

## Funções principais

1. **void init_mem**: inicializa o bloco base de memória
2. **void free_mem(void***): libera um bloco de memória previamente alocado
3. **void*** **alloc_mem(size_t)**: aloca um bloco de memória do tamanho desejado

## Diretivas

| Diretiva | Descrição                         | Default    |
|---       |---                                |---         |
| ALG_ALOG | Algoritmo de alocação escolhido   | FIRST_FIT  |
| MEM_BASE | Tamanho da memória base utilizada | 1024 (1KB) |
| DEBUG    | Ativa mensagens para depuração    | 0          |

## Constantes para cada tipo de algoritmo

|Diretiva  | Algoritmo | Constante |
|---       |---        |---        |
|FIRST_FIT | First fit | 1         |
|BEST_FIT  | Best fit  | 2         |
|WORST_FIT | Worst fit | 3         |
|NEXT_FIT  | Next fit  | 4         |

## Utilização

* Clonar o projeto

```
    mkdir teste-aloc
    cd teste-aloc
    git clone https://github.com/MarcosKrul/alloc-mem.git .
```

* Compilação e execução

```
    gcc -c alloc_mem.c -D ALG_ALOC=1
    gcc alloc_mem.o main.c -o main
    ./main
```

* Com Makefile

```
    make clean
    make TYPE=1
    make run
```
