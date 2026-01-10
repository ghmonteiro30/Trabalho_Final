#include <stdio.h>
#include <stdlib.h>
#include "fila_fifo.h"
#include "logtree.h"

int main() {
    Fila_FIFO *minhaFila = NULL;

    printf("=== Iniciando Bateria de Testes ===\n");

    /* ----------------------------------------------------------------
       TESTE 01: Inicialização
       Verifica se aloca memória e zera os ponteiros/tamanho.
    ---------------------------------------------------------------- */
    q_setup(&minhaFila);
    if (minhaFila != NULL && q_size(&minhaFila) == 0) {
        printf("Teste 01: OK\n");
    } else {
        printf("Teste 01: FALHOU\n");
    }

    /* ----------------------------------------------------------------
       TESTE 02: Inserção (Push)
       Insere 3 elementos: {10,100}, {20,200}, {30,300}.
    ---------------------------------------------------------------- */
    int i1 = q_push(&minhaFila, 10, 100);
    int i2 = q_push(&minhaFila, 20, 200);
    int i3 = q_push(&minhaFila, 30, 300);

    if (i1 == 1 && i2 == 1 && i3 == 1) {
        printf("Teste 02: OK\n");
    } else {
        printf("Teste 02: FALHOU\n");
    }

    /* ----------------------------------------------------------------
       TESTE 03: Bloqueio de Duplicatas
       Tenta inserir a chave 10 novamente. Deve retornar 0.
    ---------------------------------------------------------------- */
    int iDup = q_push(&minhaFila, 10, 999);
    if (iDup == 0) {
        printf("Teste 03: OK\n");
    } else {
        printf("Teste 03: FALHOU (Permitiu duplicata)\n");
    }

    /* ----------------------------------------------------------------
       TESTE 04: Consulta de Cabeça (Peek)
       O primeiro deve ser chave 10 e valor 100.
    ---------------------------------------------------------------- */
    if (q_peek_key(&minhaFila) == 10 && q_peek_val(&minhaFila) == 100) {
        printf("Teste 04: OK\n");
    } else {
        printf("Teste 04: FALHOU\n");
    }

    /* ----------------------------------------------------------------
       TESTE 05: Consulta por Posição
       Posição 2 deve ser {20, 200}. Posição 3 deve ser {30, 300}.
    ---------------------------------------------------------------- */
    int k2 = q_get_key_at(&minhaFila, 2);
    int v2 = q_get_val_at(&minhaFila, 2);
    int k3 = q_get_key_at(&minhaFila, 3);

    if (k2 == 20 && v2 == 200 && k3 == 30) {
        printf("Teste 05: OK\n");
    } else {
        printf("Teste 05: FALHOU\n");
    }

    /* ----------------------------------------------------------------
       TESTE 06: Verificação de Tamanho
       Esperado: 3 elementos.
    ---------------------------------------------------------------- */
    if (q_size(&minhaFila) == 3) {
        printf("Teste 06: OK\n");
    } else {
        printf("Teste 06: FALHOU\n");
    }

    /* ----------------------------------------------------------------
       TESTE 07: Remoção (Pop)
       Remove o primeiro (10). A função deve retornar a chave 10.
    ---------------------------------------------------------------- */
    int chaveRemovida = q_pop_key(&minhaFila);
    if (chaveRemovida == 10) {
        printf("Teste 07: OK\n");
    } else {
        printf("Teste 07: FALHOU\n");
    }

    /* ----------------------------------------------------------------
       TESTE 08: Estado pós-remoção
       Novo início deve ser 20, novo tamanho deve ser 2.
    ---------------------------------------------------------------- */
    if (q_peek_key(&minhaFila) == 20 && q_size(&minhaFila) == 2) {
        printf("Teste 08: OK\n");
    } else {
        printf("Teste 08: FALHOU\n");
    }

    /* ----------------------------------------------------------------
       TESTE 09: Tratamento de Erros (Posição Inválida)
       Tenta acessar posição 5 (inexistente). Deve retornar -1.
    ---------------------------------------------------------------- */
    if (q_get_key_at(&minhaFila, 5) == -1) {
        printf("Teste 09: OK\n");
    } else {
        printf("Teste 09: FALHOU\n");
    }

    /* Limpeza final (opcional, apenas para evitar memory leak no teste) */
    while(q_size(&minhaFila) > 0) {
        q_pop_key(&minhaFila);
    }
    free(minhaFila);

    /* 
       PARTE 2: TESTES DO REGISTRADOR (LOG/ABB)
        */
    Log *meuLog = NULL;
    printf("\n=== Iniciando Bateria de Testes (REGISTRADOR) ===\n");

    /* ----------------------------------------------------------------
       TESTE L01: Inicialização
    ---------------------------------------------------------------- */
    log_inicializar(&meuLog);
    if (meuLog == NULL) {
        printf("Teste Log 01: OK\n");
    } else {
        printf("Teste Log 01: FALHOU\n");
    }

    /* ----------------------------------------------------------------
       TESTE L02: Registro (Inserção)
       Cenário:
       - Conta 100: Classe 1, Espera 10
       - Conta 50:  Classe 1, Espera 20
       - Conta 150: Classe 2, Espera 30
       - Conta 200: Classe 2, Espera 50
    ---------------------------------------------------------------- */
    log_registrar(&meuLog, 100, 1, 10, 1);
    log_registrar(&meuLog, 50,  1, 20, 2);
    log_registrar(&meuLog, 150, 2, 30, 1);
    log_registrar(&meuLog, 200, 2, 50, 3);
    
    // Verifica se a raiz foi criada (Conta 100)
    if (meuLog != NULL && meuLog->conta == 100) {
        printf("Teste Log 02: OK\n");
    } else {
        printf("Teste Log 02: FALHOU\n");
    }

    /* ----------------------------------------------------------------
       TESTE L03: Contagem por Classe
       Esperado: Classe 1 = 2 clientes, Classe 2 = 2 clientes, Classe 3 = 0
    ---------------------------------------------------------------- */
    int qtdC1 = log_obter_contagem_por_classe(&meuLog, 1);
    int qtdC2 = log_obter_contagem_por_classe(&meuLog, 2);
    int qtdC3 = log_obter_contagem_por_classe(&meuLog, 3);

    if (qtdC1 == 2 && qtdC2 == 2 && qtdC3 == 0) {
        printf("Teste Log 03: OK\n");
    } else {
        printf("Teste Log 03: FALHOU (C1:%d, C2:%d, C3:%d)\n", qtdC1, qtdC2, qtdC3);
    }

    /* ----------------------------------------------------------------
       TESTE L04: Soma por Classe
       Esperado: Classe 1 (10+20=30), Classe 2 (30+50=80)
    ---------------------------------------------------------------- */
    int somaC1 = log_obter_soma_por_classe(&meuLog, 1);
    int somaC2 = log_obter_soma_por_classe(&meuLog, 2);

    if (somaC1 == 30 && somaC2 == 80) {
        printf("Teste Log 04: OK\n");
    } else {
        printf("Teste Log 04: FALHOU (Soma C1:%d, Soma C2:%d)\n", somaC1, somaC2);
    }

    /* ----------------------------------------------------------------
       TESTE L05: Média por Classe
       Esperado: Classe 1 (30/2 = 15.0), Classe 2 (80/2 = 40.0)
    ---------------------------------------------------------------- */
    float mediaC1 = log_media_por_classe(&meuLog, 1);
    float mediaC2 = log_media_por_classe(&meuLog, 2);

    if (mediaC1 == 15.0f && mediaC2 == 40.0f) {
        printf("Teste Log 05: OK\n");
    } else {
        printf("Teste Log 05: FALHOU (Media C1:%.2f, Media C2:%.2f)\n", mediaC1, mediaC2);
    }

    /* Limpeza Log */
    log_liberar(&meuLog);

    return 0;
}