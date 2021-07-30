#include "alloc_mem.h"


/* ================================================================
                        VARIAVEIS PRIVADAS                      
================================================================ */

Fragment* _list;
Fragment* _last;

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
  _last = _list; // para next fit
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

  Fragment* prev = NULL;
  Fragment* aux = _list;

  while (aux->next != NULL && aux != to_free) {
    prev = aux;
    aux = aux->next;
  }

  if (aux->next != NULL) {
    aux = aux->next;
    
    // merge com o proximo fragmento
    if (aux->size >= 0) {
      to_free->size = to_free->size + aux->size + _fragmentLength;
      to_free->next = aux->next;
    }
  }

  // merge com o fragmento anterior
  if (prev != NULL && prev->size >= 0) {
    prev->size = prev->size + to_free->size + _fragmentLength;
    prev->next = to_free->next;
  }
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

  Fragment* prev = NULL;
  Fragment* cur = _list;

  while (cur != NULL) {
    #if DEBUG
      printf("QUER ALOCAR: %ld (+%ld)\n", tam, _fragmentLength);
      printf("TAMANHO DO FRAG ATUAL: %d\n", cur->size);
    #endif

    // busca o primeiro fragmento que possua tamanho desejado: tam + estrutura de header (armazena o tamanho para liberar depois)
    if (cur->size >= ((int)(tam + _fragmentLength))) {

      prev = cur;
      cur = cur + tam;

      const int new_length = prev->size - tam;
      if (new_length > 0) {
        cur->size = new_length;
        cur->next = prev->next;
        prev->next = cur;
      }
      prev->size = (-1)* tam;

      #if DEBUG
        printf("TAMANHO FINAL: %d\n", cur->size);
      #endif

      return ((void*)prev + _fragmentLength);
    }

    cur = cur->next;
  }

  #if DEBUG
    printf("WARNING: nao foi possivel alocar memoria \n");
  #endif

  return NULL;
}

void* _alloc_next_fit(size_t tam) {
  if (tam > MEM_BASE) {
    #if DEBUG
      printf("WARNING: nao foi possivel alocar memoria \n");
    #endif
    return NULL;
  }

  Fragment* prev = NULL;
  Fragment* cur = _last;

  do {
    #if DEBUG
      printf("QUER ALOCAR: %ld (+%ld)\n", tam, _fragmentLength);
      printf("TAMANHO DO FRAG ATUAL: %d\n", cur->size);
    #endif

    // busca o primeiro fragmento que possua tamanho desejado: tam + estrutura de header (armazena o tamanho para liberar depois)
    if (cur->size >= ((int)(tam + _fragmentLength))) {

      prev = cur;
      cur = cur + tam;
      
      const int new_length = prev->size - tam;
      if (new_length > 0) {
        cur->size = new_length;
        cur->next = prev->next;
        prev->next = cur;
      }
      prev->size = (-1)* tam;

      // salva ultimo fragmento
      // proxima alocacao comeca daqui
      _last = prev;

      #if DEBUG
        printf("TAMANHO FINAL: %d\n", cur->size);
      #endif

      return ((void*)prev + _fragmentLength);
    }

    cur = cur->next == NULL? _list : cur->next;
  } while (cur != _last);

  #if DEBUG
    printf("WARNING: nao foi possivel alocar memoria \n");
  #endif

  return NULL;
}

void* _alloc_best_fit(size_t tam) { return NULL; }
void* _alloc_worst_fit(size_t tam) { return NULL; }

