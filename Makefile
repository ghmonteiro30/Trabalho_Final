
ALL: main


main: main.o fila_fifo.o escalonador.o logtree.o
	gcc main.o fila_fifo.o escalonador.o logtree.o -o main


main.o: main.c fila_fifo.h escalonador.h logtree.h
	gcc -c main.c 


fila_fifo.o: fila_fifo.c fila_fifo.h
	gcc -c fila_fifo.c 


escalonador.o: escalonador.c escalonador.h fila_fifo.h logtree.h
	gcc -c escalonador.c 


logtree.o: logtree.c logtree.h
	gcc -c logtree.c 

