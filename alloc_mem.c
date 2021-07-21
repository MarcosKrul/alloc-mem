#include "alloc_mem.h"


/* ================================================================
                        VARIAVEIS PRIVADAS                      
================================================================ */

Fragment* _list;

/*
  *
  * Tamanho do fragmento.
  * Utilizada para ajustar o ponteiro na liberacao de memoria.
  *
*/
const size_t _fragmentLength = sizeof(Fragment);


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

void init_mem() {
  _list = (Fragment*) malloc(MEM_BASE);
  if (_list == NULL) {
    printf("ERRO: alocacao do bloco base \n");
    return;
  } 
  _list->next = NULL;
  _list->size = MEM_BASE;
}

void print_mem() {
  Fragment* cur = _list;

  while (cur != NULL) {
    printf("%d - %p\n", cur->size, cur->next);
    cur = cur->next;
  }
}

void free_mem(void* ptr) {
  if (ptr == NULL) return;

  Fragment* to_free = (Fragment*) (ptr - _fragmentLength);
  if (to_free->size >= 0) {
    #if DEBUG
      printf("WARNING: bloco nao esta em uso \n"); 
    #endif
    return;
  }

  to_free->size = (-1) * to_free->size;

  // TO-DO: somar o tamanho com blocos livres adjacentes (next ou prev)
}

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
    return NULL;
  #endif
}


/* ================================================================
                        FUNCOES PRIVADAS
================================================================ */

void* _alloc_first_fit(size_t tam) {  
  if (tam > MEM_BASE) {
    #if DEBUG
      printf("WARNING: nao foi possivel alocar memoria \n");
    #endif
    return NULL;
  }

  void* aux;
  Fragment* prev = NULL;
  Fragment* cur = _list;

  #if DEBUG
    printf("QUER ALOCAR: %ld (+%ld)\n", tam, _fragmentLength);
    printf("TAMANHO INICIAL: %d\n", cur->size);
  #endif

  while (cur != NULL) {
    prev = cur;
    
    // busca o primeiro fragmento que possua tamanho desejado: tam + estrutura de header (armazena o tamanho para liberar depois)
    if (cur->size >= (tam + _fragmentLength)) {
      const int length = cur->size;

      aux = cur + cur->size - tam;
      cur = aux;
      cur->next = prev->next;
      cur->size = (-1) * tam;
      prev->next = cur;
      prev->size = length - tam - _fragmentLength;

      #if DEBUG
        printf("TAMANHO FINAL: %d\n", prev->size);
      #endif

      return (aux + _fragmentLength);
    }

    cur = cur->next;
  }

  #if DEBUG
    printf("WARNING: nao foi possivel alocar memoria \n");
  #endif

  return NULL;
}

void* _alloc_next_fit(size_t tam) { return NULL; }
void* _alloc_best_fit(size_t tam) { return NULL; }
void* _alloc_worst_fit(size_t tam) { return NULL; }

