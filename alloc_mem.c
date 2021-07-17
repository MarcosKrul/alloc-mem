#include "alloc_mem.h"

/* ================================================================
                  PROTOTIPOS FUNCOES PRIVADAS                      
================================================================ */

/*
 *
 * Funcoes de alocacao.
 * A funcao alloc_mem ira chamar alguma delas,
 * dependendo do tipo de alocacao escolhido (ALG_ALOC).
 *
*/
void* _alloc_next_fit(size_t);
void* _alloc_best_fit(size_t);
void* _alloc_first_fit(size_t);
void* _alloc_worst_fit(size_t);


/* ================================================================
                        FUNCOES PRINCIPAIS                      
================================================================ */


void* alloc_mem(size_t tam) {
  #if ALG_ALOC == FIRST_FIT
    return _alloc_first_fit(tam);
  #elif ALG_ALOC == NEXT_FIT 
    return _alloc_next_fit(tam);
  #elif ALG_ALOC == BEST_FIT 
    return _alloc_best_fit(tam);
  #elif ALG_ALOC == WORST_FIT 
    return _alloc_worst_fit(tam);
  #else
    printf("ERRO: algoritmo de alocacao nao suportado \n");
  #endif
}


/* ================================================================
                        FUNCOES PRIVADAS
================================================================ */

void* _alloc_first_fit(size_t tam) { return NULL; }
void* _alloc_next_fit(size_t tam) { return NULL; }
void* _alloc_best_fit(size_t tam) { return NULL; }
void* _alloc_worst_fit(size_t tam) { return NULL; }

