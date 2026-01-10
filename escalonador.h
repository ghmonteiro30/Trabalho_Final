#ifndef ESCALONADOR_H
#define ESCALONADOR_H
#include "fila_fifo.h"

typedef struct _escalonador{
	Fila_FIFO *fila[5];
	int disciplina[5];
	int *caixa;
	int delta_t;
	int idx_disciplina; // Indice da disciplina atual
	int clientes_restantes; // Clientes restantes acordo com a disciplina
} Escalonador;

typedef enum {
	CLASSE_PREMIUM,
	CLASSE_OURO,
	CLASSE_PRATA,
	CLASSE_BRONZE,
	CLASSE_LEEZU,
	CLASSE_DESCONHECIDA
} TipoCategoria;

static const char *NOME_CATEGORIA[] = {
	"Premium",
	"Ouro",
	"Prata",
	"Bronze",
	"Leezu"
};

void e_inicializar(Escalonador **e, int caixas, int delta_t, int n_1, int n_2, int n_3, int n_4, int n_5);
int e_inserir_por_fila(Escalonador *e, int classe, int num_conta, int qtde_operacoes);
int e_obter_prox_num_conta(Escalonador *e);
int e_consultar_prox_num_conta(Escalonador *e);
int e_consultar_prox_qtde_oper(Escalonador *e);
int e_consultar_prox_fila(Escalonador *e);
int e_consultar_qtde_clientes(Escalonador *e);
int e_consultar_tempo_prox_cliente(Escalonador *e);
int e_conf_por_arquivo(Escalonador **e, char *nome_arq_conf);
void e_rodar(Escalonador **e, char *nome_arq_in, char *nome_arq_out);

// Se a próxima fila ficou vazia, pula a disciplina de atendimento para a próxima fila não vazia
void e_pular_filas_vazias(Escalonador *e);
// Função para pegar o indice dado o nome da categoria
TipoCategoria get_idx_classe(char *nome);

#endif