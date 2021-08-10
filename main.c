#include <stdio.h>
#include "alloc_mem.h"

int main() {
  init_mem();
  void *a1 = alloc_mem(10);
  void *a2 = alloc_mem(5);
  void *a3 = alloc_mem(20);
  void *a4 = alloc_mem(30);
  void *a5 = alloc_mem(15);
  void *a6 = alloc_mem(40);
  void *a7 = alloc_mem(60);
  void *a8 = alloc_mem(80);
  void *a9 = alloc_mem(100);
  print_mem();
  free_mem(a2);
  free_mem(a4);
  free_mem(a5);
  free_mem(a7);
  free_mem(a9);
  print_mem();
  void* a10 =  alloc_mem(3);
  print_mem();
  void* a11 =  alloc_mem(20);
  print_mem();
  void* a12 =  alloc_mem(12);
  print_mem();
  void* a13 =  alloc_mem(40);
  print_mem();
}