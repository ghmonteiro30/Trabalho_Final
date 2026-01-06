#include <stdlib.h>
#include "fila_fifo.h"

/* Inicializa a estrutura da fila */
void q_setup(Fila_FIFO **q) {
    Fila_FIFO *temp = (Fila_FIFO *) malloc(sizeof(Fila_FIFO));
    if (temp) {
        temp->inicio = NULL;
        temp->fim = NULL;
        temp->tamanho = 0;
        *q = temp;
    }
}

/* Insere novo elemento se a chave for unica */
int q_push(Fila_FIFO **q, int key, int val) {
    if (!q || !*q) return 0;

    /* Verificação de unicidade usando loop for */
    for (NoFila *iter = (*q)->inicio; iter != NULL; iter = iter->prox) {
        if (iter->chave == key) return 0;
    }

    NoFila *node = (NoFila *) malloc(sizeof(NoFila));
    if (!node) return 0;

    node->chave = key;
    node->valor = val;
    node->prox = NULL;

    /* Lógica de inserção simplificada */
    if ((*q)->fim) {
        (*q)->fim->prox = node;
    } else {
        (*q)->inicio = node;
    }
    
    (*q)->fim = node;
    (*q)->tamanho++;

    return 1;
}

/* Remove o primeiro elemento e retorna sua chave */
int q_pop_key(Fila_FIFO **q) {
    if (!q || !*q || !(*q)->inicio) return -1;

    NoFila *target = (*q)->inicio;
    int k = target->chave;

    (*q)->inicio = target->prox;
    
    if (!(*q)->inicio) {
        (*q)->fim = NULL;
    }

    free(target);
    (*q)->tamanho--;

    return k;
}

/* Consulta a chave do início sem remover */
int q_peek_key(Fila_FIFO **q) {
    if (q && *q && (*q)->inicio) {
        return (*q)->inicio->chave;
    }
    return -1;
}

/* Consulta o valor do início sem remover */
int q_peek_val(Fila_FIFO **q) {
    if (q && *q && (*q)->inicio) {
        return (*q)->inicio->valor;
    }
    return -1;
}

/* Retorna tamanho atual */
int q_size(Fila_FIFO **q) {
    return (*q) ? (*q)->tamanho : 0;
}

/* Busca chave por índice (base 1) */
int q_get_key_at(Fila_FIFO **q, int idx) {
    if (!q || !*q || idx < 1) return -1;

    NoFila *cursor = (*q)->inicio;
    int count = 1;

    while (cursor) {
        if (count == idx) return cursor->chave;
        cursor = cursor->prox;
        count++;
    }
    return -1;
}

/* Busca valor por índice (base 1) */
int q_get_val_at(Fila_FIFO **q, int idx) {
    if (!q || !*q || idx < 1) return -1;

    NoFila *cursor = (*q)->inicio;
    
    for (int i = 1; cursor != NULL; i++) {
        if (i == idx) return cursor->valor;
        cursor = cursor->prox;
    }
    
    return -1;
}