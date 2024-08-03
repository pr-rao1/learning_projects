GCC_FLAG = -g

all: final

final: hash_table.o prime.o
	gcc $(GCC_FLAG) hash_table.o prime.o -o hash_bin.exe

hash_table.o: hash_table.c prime.o
	gcc $(GCC_FLAG) -c hash_table.c

prime.o: prime.c
	gcc $(GCC_FLAG) -c prime.c

clean:
	del hash_bin.exe hash_table.o prime.o

debug: hash_table.c prime.o
	gcc $(GCC_FLAG) -c -D DEBUG_PRINTS hash_table.c
	make final