#include "alloc_mem.h"


/* ================================================================
                        ESTRUTURA DE DADOS                      
================================================================ */

/*
 *
 * Fragment.
 * Estrutura para lista de fragmentos da memoria.
 *
*/
typedef struct fragment_t {
  int size;
  struct fragment_t* next;
} Fragment;


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
int _digits(int);
void _padding(int);
char* _get_label(int);
unsigned int _mod(int);
void* _alloc_next_fit(size_t);
void* _alloc_first_fit(size_t);
void* _alloc_best_worst_fit(size_t);
void* _handle_make_ptr(size_t,Fragment*);


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
  int used = 0;
  Fragment* cur = _list;

  printf("+--------+---------+----------------------------------+\n");
  printf("| Em uso | Tamanho | Endereco -> Proximo              |\n");
  printf("+--------+---------+----------------------------------+\n");
  while (cur != NULL) {
    printf("|  %s   | %d", _get_label(cur->size), _mod(cur->size)); 
    _padding((_digits(MEM_BASE) > 7? _digits(MEM_BASE) : 7) - _digits(cur->size)); 
    if (cur->next == NULL) printf(" | %p -> NULL           |\n", cur);
    else printf(" | %p -> %p |\n", cur, cur->next);
    printf("+--------+---------+----------------------------------+\n");
    if (cur->size < 0) used += cur->size;
    cur = cur->next;
  }
  printf("| TAMANHO DO BLOCO TOTAL: %d", MEM_BASE); _padding(28-_digits(MEM_BASE)); printf("|\n");
  printf("+-----------------------------------------------------+\n");
  printf("| TOTAL EM USO: %d", _mod(used)); _padding(38-_digits(used)); printf("|\n");
  printf("+-----------------------------------------------------+\n\n");
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
  #elif ALG_ALOC == BEST_FIT || ALG_ALOC == WORST_FIT
    return _alloc_best_worst_fit(tam);
  #else
    printf("ERRO: algoritmo de alocacao nao suportado \n");
    return NULL;
  #endif
}


/* ================================================================
                        FUNCOES PRIVADAS
================================================================ */

int _digits(int value) {
  if (value == 0) return 1;

  int aux = 0;
  while (value != 0) {
    value = value / 10;
    aux++;
  }

  return aux;
}

char* _get_label(int tam) { return tam > 0 ? "Nao" : "Sim"; }

void _padding(int x) { for(int i=0 ; i<x ; i++) printf(" "); }

unsigned int _mod(int value) { return value > 0? value : (-1)*value; }


void* _handle_make_ptr(size_t tam, Fragment* ptr) {
  Fragment* aux = ptr;  
  ptr = (Fragment*) ((void*) ptr + tam + _fragmentLength);

  const int new_length = aux->size - tam - _fragmentLength;
  if (new_length > 0) {
    ptr->size = new_length;
    ptr->next = aux->next;
    aux->next = ptr;
  }
  aux->size = (-1)* tam;

  // para next fit
  // salva ultimo fragmento
  // proxima busca comeca daqui
  #if ALG_ALOC == NEXT_FIT
    _last = aux;
  #endif

  #if DEBUG
    printf("TAMANHO FINAL: %d\n", ptr->size);
  #endif

  return ((void*)aux + _fragmentLength);
}

void* _alloc_first_fit(size_t tam) {  
  if (tam > MEM_BASE) {
    #if DEBUG
      printf("WARNING: nao foi possivel alocar memoria \n");
    #endif
    return NULL;
  }

  Fragment* cur = _list;

  while (cur != NULL) {
    #if DEBUG
      printf("QUER ALOCAR: %ld (+%ld)\n", tam, _fragmentLength);
      printf("TAMANHO DO FRAG ATUAL: %d\n", cur->size);
    #endif

    // busca o primeiro fragmento que possua tamanho desejado: 
    // tam + estrutura de header (armazena o tamanho para liberar depois)
    if (cur->size >= ((int)(tam + _fragmentLength))) {
      return _handle_make_ptr(tam,cur);
    } else if (cur->size >= (int) tam) {
      cur->size = (-1)* cur->size;
      return ((void*) cur + _fragmentLength);
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

  Fragment* cur = _last;

  do {
    #if DEBUG
      printf("QUER ALOCAR: %ld (+%ld)\n", tam, _fragmentLength);
      printf("TAMANHO DO FRAG ATUAL: %d\n", cur->size);
    #endif

    // busca o primeiro fragmento que possua tamanho desejado, a partir do ultimo
    if (cur->size >= ((int)(tam + _fragmentLength))) {
      return _handle_make_ptr(tam,cur);
    } else if (cur->size >= (int) tam) {
      cur->size = (-1)* cur->size;
      _last = cur;
      return ((void*) cur + _fragmentLength);
    }

    cur = cur->next == NULL? _list : cur->next;
  } while (cur != _last);

  #if DEBUG
    printf("WARNING: nao foi possivel alocar memoria \n");
  #endif

  return NULL;
}

void* _alloc_best_worst_fit(size_t tam) { 
  if (tam > MEM_BASE) {
    #if DEBUG
      printf("WARNING: nao foi possivel alocar memoria \n");
    #endif
    return NULL;
  }

  #if ALG_ALOC == BEST_FIT
  int sm = MEM_BASE;
  #elif ALG_ALOC == WORST_FIT 
  int sm = 0;
  #else
    #if DEGUB
      printf("ERROR: mapeamento funcao-algoritmo errado \n");
    #endif
    return NULL;
  #endif

  Fragment* aux = NULL;
  Fragment* cur_prev = _list;

  while (cur_prev != NULL) {
    #if ALG_ALOC == BEST_FIT || ALG_ALOC == WORST_FIT
    #if ALG_ALOC == BEST_FIT
    if (cur_prev->size >= ((int)(tam + _fragmentLength)) && cur_prev->size <= sm) {
    #elif ALG_ALOC == WORST_FIT 
    if (cur_prev->size >= ((int)(tam + _fragmentLength)) && cur_prev->size >= sm) {
    #endif
      sm = cur_prev->size;
      aux = cur_prev;
    }
    #endif
    cur_prev = cur_prev->next;
  }

  if (aux == NULL) {
    #if DEBUG
      printf("WARNING: nao foi possivel alocar memoria \n");
    #endif
    return NULL;
  }

  if (aux->size >= ((int)(tam + _fragmentLength))) return _handle_make_ptr(tam,aux);
  else {
    aux->size = (-1)* aux->size;
    return ((void*) aux + _fragmentLength);
  }
}

