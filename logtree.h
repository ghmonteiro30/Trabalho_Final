#ifndef LOGTREE_H
#define LOGTREE_H

// Estrutura do Nó da Árvore de Log (ABB) 
typedef struct LogNo {
    int conta;          // Chave da árvore (ÚNICA)
    int classe;         // Categoria (1 a 5)
    int timer;          // Tempo de espera
    int caixa;          // Qual caixa atendeu
    struct LogNo *esq;
    struct LogNo *dir;
} Log;

// Funções pedidas
void log_inicializar(Log **l);
void log_registrar(Log **l, int conta, int classe, int timer, int caixa);
float log_media_por_classe(Log **l, int classe);
int log_obter_soma_por_classe(Log **l, int classe);
int log_obter_contagem_por_classe(Log **l, int classe);

// Função auxiliar para limpar memória 
void log_liberar(Log **l);

#endif