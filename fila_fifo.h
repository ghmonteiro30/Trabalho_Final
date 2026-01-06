typedef struct no_fila {
    int chave;
    int valor;
    struct no_fila *prox;
} NoFila;

typedef struct {
    NoFila *inicio;
    NoFila *fim;
    int tamanho;
} Fila_FIFO;

void q_setup(Fila_FIFO **q);
int q_push(Fila_FIFO **q, int key, int val);
int q_pop_key(Fila_FIFO **q);
int q_peek_key(Fila_FIFO **q);
int q_peek_val(Fila_FIFO **q);
int q_size(Fila_FIFO **q);
int q_get_key_at(Fila_FIFO **q, int idx);
int q_get_val_at(Fila_FIFO **q, int idx);