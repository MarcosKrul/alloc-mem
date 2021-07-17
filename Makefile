# define o tipo de algoritmo para alocacao
# FIRST_FIT -> make TYPE=1
# BEST_FIT  -> make TYPE=2
# WORST_FIT -> make TYPE=3
# NEXT_FIT  -> make TYPE=4
TYPE=1

all: alloc_mem.o
	gcc alloc_mem.o main.c -o main

alloc_mem.o: alloc_mem.c
	gcc -c alloc_mem.c -D ALG_ALOC=$(TYPE)

clean:
	rm -rf *.o main

run:
	./main