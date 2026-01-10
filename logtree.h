#ifndef LOGTREE_H
#define LOGTREE_H

// Estrutura do Nó da Árvore de Log (ABB) 
typedef struct LogNo {
    int conta;          // Chave da árvore (ÚNICA)
    int classe;         // Categoria (1 a 5)
    int timer;          // Tempo de espera
    int qtde_operacoes; // Quantidade de operações realizadas
    int caixa;          // Qual caixa atendeu
    struct LogNo *esq;
    struct LogNo *dir;
} Log;

// Funções pedidas
void log_inicializar(Log **l);
void log_registrar(Log **l, int conta, int classe, int timer, int caixa, int qtde_operacoes);

float log_media_por_classe(Log **l, int classe);
float log_media_operacoes_por_classe(Log **l, int classe);

int log_obter_soma_por_classe(Log **l, int classe);
int log_obter_total_operacoes_por_classe(Log **l, int classe);
int log_obter_contagem_por_classe(Log **l, int classe);

// Função extra para contar clientes por caixa
int log_obter_contagem_por_caixa(Log **l, int caixa);

// Função auxiliar para limpar memória 
void log_liberar(Log **l);

#endif