#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "escalonador.h"

void e_inicializar(Escalonador **e, int caixas, int delta_t, int n_1, int n_2, int n_3, int n_4, int n_5){
	Escalonador *temp = (Escalonador*)malloc(sizeof(Escalonador));
	int i;
	
	if (temp){
		temp->caixas = (int*)malloc(sizeof(int));
		temp->delta_t = delta_t;
		for (i = 0; i < 5; i++) {
			temp->fila[i] = NULL;
			q_setup(&temp->fila[i]);
		}
		temp->disciplina[0] = n_1;
		temp->disciplina[1] = n_2;
		temp->disciplina[2] = n_3;
		temp->disciplina[3] = n_4;
		temp->disciplina[4] = n_5;
		temp->idx_disciplina = 0;
		temp->clientes_restantes = temp->disciplina[temp->idx_disciplina];
		*e = temp;
	}
}

int e_inserir_por_fila(Escalonador *e, int classe, int num_conta, int qtde_operacoes){
	return q_push(&e->fila[classe-1], num_conta, qtde_operacoes);
}

int e_obter_prox_num_conta(Escalonador *e){
	int num_conta, i = 0;	
	// Verifica se todas as filas estão vazias
	num_conta = q_peek_key(&e->fila[e->idx_disciplina]);
	while (num_conta == -1) {
		if (e->idx_disciplina == 4)
			e->idx_disciplina = 0;
		else
			e->idx_disciplina++;
		num_conta = q_peek_key(&e->fila[e->idx_disciplina]);
		e->clientes_restantes = e->disciplina[e->idx_disciplina];
		i++;
		if (num_conta == -1 && i == 4)
			return -1;
	}
	
	
	num_conta = q_pop_key(&e->fila[e->idx_disciplina]);
	e->clientes_restantes--;
	if (e->clientes_restantes == 0) {
		if (e->idx_disciplina == 4)
			e->idx_disciplina = 0;
		else
			e->idx_disciplina++;
		e->clientes_restantes = e->disciplina[e->idx_disciplina];
	}
	return num_conta;
}

int e_consultar_prox_num_conta(Escalonador *e){
	return q_peek_key(&e->fila[e->idx_disciplina]);
}

int e_consultar_prox_qtde_oper(Escalonador *e){
	return q_peek_val(&e->fila[e->idx_disciplina]);
}

int e_consultar_prox_fila(Escalonador *e){
	return e->idx_disciplina + 1; // +1 pois os indices vão de 0 a 4, mas as filas são nomeadas de 1 a 5
}

int e_consultar_qtde_clientes(Escalonador *e){
	int num_clientes, i;
	
	num_clientes = 0;
	for (i = 0; i < 5; i++)
		num_clientes = num_clientes + q_size(&e->fila[i]);
	
	return num_clientes;
}

int e_consultar_tempo_prox_cliente(Escalonador *e){
	int qtde, i;
	
	// Verifica se todas as filas estão vazias
	i = 0;
	qtde = q_peek_val(&e->fila[i]);
	while (qtde == -1) {
		i++;
		qtde = q_peek_val(&e->fila[i]);
		if (qtde == -1 && i == 4)
			return -1;
	}
	// Se alguma fila não estiver vazia, será onde o próximo cliente estará de acordo com a disciplina de escalonamento
	return e->delta_t * q_peek_val(&e->fila[e->idx_disciplina]);
}

int e_conf_por_arquivo(Escalonador **e, char *nome_arq_conf){
	int caixas, delta_t, n_1, n_2, n_3, n_4, n_5;
	FILE *arq; 
	
	arq = fopen(nome_arq_conf, "rt");
	if (arq == NULL)
		return 0;
	fscanf(arq, "%*[^0-9]%d", &caixas);
	fscanf(arq, "%*[^0-9]%d", &delta_t);
	fscanf(arq, "%*[^0-9]%d%*[^0-9]%d%*[^0-9]%d%*[^0-9]%d%*[^0-9]%d%*c", &n_1, &n_2, &n_3, &n_4, &n_5);
	
	e_inicializar(e, caixas, delta_t, n_1, n_2, n_3, n_4, n_5);
	fclose(arq);
	return 1;
}

void e_rodar(Escalonador **e, char *nome_arq_in, char *nome_arq_out){
	int caixas, delta_t, n_1, n_2, n_3, n_4, n_5, num_conta, classe, oper, i, j, rodadas;
	FILE *arq; 
	char buffer[10];
	
	arq = fopen(nome_arq_in, "rt");
	if (arq == NULL) {
		printf("Nao foi possível abrir o arquivo!\n");
		return;
	}
	fscanf(arq, "%*[^0-9]%d", &caixas);
	fscanf(arq, "%*[^0-9]%d", &delta_t);
	fscanf(arq, "%*[^0-9]%d%*[^0-9]%d%*[^0-9]%d%*[^0-9]%d%*[^0-9]%d%*c", &n_1, &n_2, &n_3, &n_4, &n_5);
	e_inicializar(e, caixas, delta_t, n_1, n_2, n_3, n_4, n_5);
	
	while (fscanf(arq, "%s - conta %d - %d operacao(oes)", buffer, &num_conta, &oper) != EOF) {
		if (strcmp("Premium", buffer) == 0)
			classe = 1;
		if (strcmp("Ouro", buffer) == 0)
			classe = 2;
		if (strcmp("Prata", buffer) == 0)
			classe = 3;
		if (strcmp("Bronze", buffer) == 0)
			classe = 4;
		if (strcmp("Leezu", buffer) == 0)
			classe = 5;
		e_inserir_por_fila(*e, classe, num_conta, oper);
	}
	fclose(arq);

	while (e_consultar_qtde_clientes(*e) > 0) {
		for (j = 0; j < caixas; j++)
			printf("%d\n", e_obter_prox_num_conta(*e));
	}
	
}