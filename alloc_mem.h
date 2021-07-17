#ifndef ALLOC_MEM_
#define ALLOC_MEM_


// dependencias
#include <stdio.h>
#include <stdlib.h>



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
 * Fragment.
 * Estrutura para lista de fragmentos da memoria.
 *
*/
typedef struct fragment_t {
  size_t size;
  struct fragment_t* next;
} Fragment;


#endif