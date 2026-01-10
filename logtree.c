#include <stdio.h>
#include <stdlib.h>
#include "logtree.h"

void log_inicializar(Log **l) {
    *l = NULL;
}

// Inclui um registro na ABB ordenado pela conta 
void log_registrar(Log **l, int conta, int classe, int timer, int caixa) {
    // Se encontrou posição vazia, insere 
    if (*l == NULL) {
        Log *novo = (Log *) malloc(sizeof(Log));
        if (novo) {
            novo->conta = conta;
            novo->classe = classe;
            novo->timer = timer;
            novo->caixa = caixa;
            novo->esq = NULL;
            novo->dir = NULL;
            *l = novo;
        }
        return;
    }

    // Navegação da ABB pela chave 'conta' 
    if (conta < (*l)->conta) {
        log_registrar(&(*l)->esq, conta, classe, timer, caixa);
    } else if (conta > (*l)->conta) {
        log_registrar(&(*l)->dir, conta, classe, timer, caixa);
    }
    
}

// Retorna a quantidade de clientes de uma classe específica 
int log_obter_contagem_por_classe(Log **l, int classe) {
    if (*l == NULL) return 0;

    int qtd = 0;
    // Verifica o nó atual 
    if ((*l)->classe == classe) {
        qtd = 1;
    }

    // Soma recursiva com as subárvores 
    return qtd + log_obter_contagem_por_classe(&(*l)->esq, classe)
               + log_obter_contagem_por_classe(&(*l)->dir, classe);
}

// Retorna a quantidade de clientes de uma caixa específico
int log_obter_contagem_por_caixa(Log **l, int caixa) {
	if (*l == NULL) return 0;

    int qtd = 0;
    // Verifica o nó atual 
    if ((*l)->caixa == caixa) {
        qtd = 1;
    }

    // Soma recursiva com as subárvores 
    return qtd + log_obter_contagem_por_caixa(&(*l)->esq, caixa)
               + log_obter_contagem_por_caixa(&(*l)->dir, caixa);
}

// Retorno da soma dos tempos de espera de uma classe 
int log_obter_soma_por_classe(Log **l, int classe) {
    if (*l == NULL) return 0;

    int soma = 0;
    // Se for da classe alvo, soma o timer 
    if ((*l)->classe == classe) {
        soma = (*l)->timer;
    }

    // Soma recursiva 
    return soma + log_obter_soma_por_classe(&(*l)->esq, classe)
                + log_obter_soma_por_classe(&(*l)->dir, classe);
}

// Retorna a média de espera para a classe 
float log_media_por_classe(Log **l, int classe) {
    int soma = log_obter_soma_por_classe(l, classe);
    int qtd = log_obter_contagem_por_classe(l, classe);

    if (qtd == 0) return 0.0;

    return (float)soma / qtd;
}

// Liberar memória 
void log_liberar(Log **l) {
    if (*l != NULL) {
        log_liberar(&(*l)->esq);
        log_liberar(&(*l)->dir);
        free(*l);
        *l = NULL;
    }
}