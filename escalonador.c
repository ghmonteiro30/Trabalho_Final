#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "escalonador.h"
#include "logtree.h"

void e_inicializar(Escalonador **e, int caixas, int delta_t, int n_1, int n_2, int n_3, int n_4, int n_5){
	Escalonador *temp = (Escalonador*)malloc(sizeof(Escalonador));
	int i;
	
	if (temp){
		temp->caixa = (int*)malloc(sizeof(int));
		for (i = 0; i < caixas; i++)
			temp->caixa[i] = 0;
		for (i = 0; i < 5; i++) {
			temp->fila[i] = NULL;
			q_setup(&temp->fila[i]);
		}
		temp->delta_t = delta_t;
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

void e_pular_filas_vazias(Escalonador *e){
	int num_conta, i = 0;	

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
			return;// Sai se todas as listas estiverem vazias
	}
}

int e_inserir_por_fila(Escalonador *e, int classe, int num_conta, int qtde_operacoes){
	return q_push(&e->fila[classe-1], num_conta, qtde_operacoes);
}

int e_obter_prox_num_conta(Escalonador *e){
	int num_conta;
	
	e_pular_filas_vazias(e);
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
	e_pular_filas_vazias(e);
	return q_peek_key(&e->fila[e->idx_disciplina]);
}

int e_consultar_prox_qtde_oper(Escalonador *e){
	e_pular_filas_vazias(e);
	return q_peek_val(&e->fila[e->idx_disciplina]);
}

int e_consultar_prox_fila(Escalonador *e){
	e_pular_filas_vazias(e);
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
	e_pular_filas_vazias(e);
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
	int caixas, delta_t, n_1, n_2, n_3, n_4, n_5, num_conta, classe, oper, i, rodadas, caixa_livre, timer, tempo_total = 0;
	Log *registrador;
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
		classe = get_idx_classe(buffer) + 1;
		e_inserir_por_fila(*e, classe, num_conta, oper);
	}
	fclose(arq);
	
	arq = fopen(nome_arq_out, "wt");
	log_inicializar(&registrador);
		
	while (e_consultar_qtde_clientes(*e) > 0) {
		caixa_livre = 0;
		for (i = 1; i < caixas; i++)
			if ((*e)->caixa[i] < (*e)->caixa[caixa_livre])
				caixa_livre = i;			
		num_conta = e_consultar_prox_num_conta(*e);
		oper = e_consultar_prox_qtde_oper(*e);
		classe = (*e)->idx_disciplina;
		timer = (*e)->caixa[caixa_livre];
		
		
		log_registrar(&registrador, num_conta, classe, timer, caixa_livre);
		
		printf("T = %d min: Caixa %d chama da categoria %s cliente da conta %d para realizar %d operacoes\n", timer, caixa_livre+1, NOME_CATEGORIA[classe], num_conta, oper);
		(*e)->caixa[caixa_livre] = timer + ((e_consultar_prox_qtde_oper(*e) * (*e)->delta_t));
		tempo_total = tempo_total + ((*e)->caixa[caixa_livre] - tempo_total);
		
		e_obter_prox_num_conta(*e);
	}
	printf("Tempo total: %d\n", tempo_total);
	printf("Tempo medio de espera dos %d clientes Premium: %g\n", log_obter_contagem_por_classe(&registrador, 0), log_media_por_classe(&registrador, 0));
	printf("Tempo medio de espera dos %d clientes Ouro: %g\n", log_obter_contagem_por_classe(&registrador, 1), log_media_por_classe(&registrador, 1));
	printf("Tempo medio de espera dos %d clientes Prata: %g\n", log_obter_contagem_por_classe(&registrador, 2), log_media_por_classe(&registrador, 2));
	printf("Tempo medio de espera dos %d clientes Bronze: %g\n", log_obter_contagem_por_classe(&registrador, 3), log_media_por_classe(&registrador, 3));
	printf("Tempo medio de espera dos %d clientes Leezu: %g\n", log_obter_contagem_por_classe(&registrador, 4), log_media_por_classe(&registrador, 4));
	for (i = 0; i < caixas; i++)
		printf("O caixa de número %d atendeu %d clientes.\n", i+1, 0);
}

TipoCategoria get_idx_classe(char *nome) {
	if (strcmp(nome, "Premium") == 0)
		return CLASSE_PREMIUM;
	if (strcmp(nome, "Ouro") == 0)
		return CLASSE_OURO;
	if (strcmp(nome, "Prata") == 0)
		return CLASSE_PRATA;
	if (strcmp(nome, "Bronze") == 0)
		return CLASSE_BRONZE;
	if (strcmp(nome, "Leezu") == 0)
		return CLASSE_LEEZU;
	return CLASSE_DESCONHECIDA;
}