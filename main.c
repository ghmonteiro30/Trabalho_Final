#include <stdio.h>
#include <stdlib.h>
#include "fila_fifo.h"
#include "logtree.h"
#include "escalonador.h"

int main(int narg, char *argv[]) {
	Escalonador *e = NULL;
	char entrada[17], saida[15];
	int x;
	
	do{
		printf("Número da entrada[1 a 9999]: ");
		scanf("%d", &x);
	} while (x < 1 || x > 9999);
	
	sprintf(entrada, "entrada-%04d.txt", x);
	sprintf(saida, "saida-%04d.txt", x);
	
	e_rodar(&e, entrada, saida);

    return EXIT_SUCCESS;
}