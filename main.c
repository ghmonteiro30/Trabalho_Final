#include <stdio.h>
#include <stdlib.h>
#include "fila_fifo.h"
#include "logtree.h"
#include "escalonador.h"

int main(int narg, char *argv[]) {
	Escalonador *e = NULL;
	
	e_rodar(&e, argv[1], argv[2]);

    return EXIT_SUCCESS;
}