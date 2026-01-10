/* Trabalho Final – Banco da Cochinchina do Sul - Disciplina Programação (CK0226) 
   Autores: ANTONIO CARDOSO DO NASCIMENTO NETO - 508199
			FRANCISCO DAVID VAZ DE SOUSA - 554152
			GABRIEL HENRIQUE MONTEIRO DE MORAIS - 548113

   Data: Janeiro de 2026
   Descrição: O programa começa recebendo um número x de 1 a 9999, que será usado para identificar
   o arquivo de entrada e nomear o arquivo de saida no formato entrada-X.txt e saida-X.txt, respectivamente.
   O programa então executa a simulação de atendimento com os dados recebidos na entrada e gera a saída

   CONTRIBUIÇÕES DOS AUTORES:
	- ANTONIO CARDOSO DO NASCIMENTO NETO: Implementação do escalonador

	- FRANCISCO DAVID VAZ DE SOUSA: Implementação do registrador

	- GABRIEL HENRIQUE MONTEIRO DE MORAIS: Implementação da fila
*/

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