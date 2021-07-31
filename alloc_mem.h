#ifndef ALLOC_MEM_
#define ALLOC_MEM_


// dependencias
#include <stdio.h>
#include <stdlib.h>


#define DEBUG 0


// algoritmos de alocacao
#define FIRST_FIT 1
#define BEST_FIT 2
#define WORST_FIT 3
#define NEXT_FIT 4

/*
  *
  * ALG_ALOC
  * Define qual algoritmo sera utilizado para alocar memoria.
  * 
  * FIRST_FIT - escolhe o primeiro segmento para alocar.
  * NEXT_FIT  - derivado do first fit, com implemetacao de uma lista circular.
  * BEST_FIT  - o segmento escolhido eh o qual cujo resto deve ser o menor possivel.
  * WORST_FIT - o segmento escolhido eh o qual cujo resto deve ser o maior possivel.
  *
*/
#ifndef ALG_ALOC
#define ALG_ALOC FIRST_FIT
#endif

/* 
 *
 * MEM_BASE
 * Define o tamanho da memoria base utilizada.
 * Default: 1KB
 *
*/
#define MEM_BASE 1024


/*
 *
 * init_mem
 * Inicializa o bloco de memoria.
 *
*/
void init_mem();

/*
 *
 * alloc_mem
 * Aloca um bloco de memoria.
 * Parametro 1: tamanho do bloco.
 * 
*/
void* alloc_mem(size_t);

/*
 *
 * free_mem
 * Libera um fragmento de memoria
 * alocado anteriormente.
 * Parametro 1: ponteiro para o fragmento.
 *
*/
void free_mem(void*);

/*
 *
 * print_mem
 * Funcao auxiliar, define uma representacao
 * grafica para a lista de fragmentos.
 *
*/
void print_mem();

#endif