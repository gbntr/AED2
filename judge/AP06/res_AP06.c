/*
 * AED2 2026 (1S) - AP-06
 * Tabela Hash com encadeamento separado usando listas duplamente encadeadas
 * ordenadas (crescente). Busca bidirecional otimizada por NEP.
 */

#include <stdio.h>
#include <stdlib.h>

/* ── Estruturas ──────────────────────────────────────────────── */

typedef struct Node {
    int key;
    struct Node *prev, *next;
} Node;

typedef struct {
    Node *head, *tail;
} List;

/* ── Utilitários de nó ───────────────────────────────────────── */

Node *new_node(int key) {
    Node *n = malloc(sizeof(Node));
    n->key = key;
    n->prev = n->next = NULL;
    return n;
}

/* ── Inserção ordenada (Insertion Sort) ──────────────────────── */

void insert(List *list, int key) {
    Node *n = new_node(key);

    /* lista vazia */
    if (!list->head) {
        list->head = list->tail = n;
        return;
    }

    /* acha posição: primeiro nó >= key */
    Node *cur = list->head;
    while (cur && cur->key < key)
        cur = cur->next;

    if (!cur) {
        /* insere no fim */
        n->prev = list->tail;
        list->tail->next = n;
        list->tail = n;
    } else if (!cur->prev) {
        /* insere no início */
        n->next = list->head;
        list->head->prev = n;
        list->head = n;
    } else {
        /* insere no meio */
        n->prev = cur->prev;
        n->next = cur;
        cur->prev->next = n;
        cur->prev = n;
    }
}

/* ── Remoção de um nó específico ─────────────────────────────── */

void remove_node(List *list, Node *n) {
    if (n->prev) n->prev->next = n->next;
    else         list->head    = n->next;

    if (n->next) n->next->prev = n->prev;
    else         list->tail    = n->prev;

    free(n);
}

/* ── Busca bidirecional otimizada com NEP ────────────────────── */
/*
 * Decide a direção comparando |k - H| vs |T - k|.
 * Percorre a lista e aplica corte lógico:
 *   - sentido crescente (head→tail): para se cur->key > key (impossível encontrar)
 *   - sentido decrescente (tail→head): para se cur->key < key
 * Remove todas as ocorrências encontradas.
 * Retorna o NEP e preenche *sentido e *encontrado.
 */
int search_and_remove(List *list, int key, int *sentido, int *encontrado) {
    *encontrado = 0;

    if (!list->head) {
        *sentido = 0; /* lista vazia */
        return 0;
    }

    int H = list->head->key;
    int T = list->tail->key;
    int dHead = (key >= H) ? (key - H) : (H - key);
    int dTail = (T >= key) ? (T - key) : (key - T);

    /* empate ou head mais próximo → começa pelo início */
    int from_head = (dHead <= dTail);
    *sentido = from_head ? 1 : 2;

    int nep = 0;

    if (from_head) {
        Node *cur = list->head;
        while (cur) {
            nep++;
            if (cur->key == key) {
                *encontrado = 1;
                Node *nxt = cur->next;
                remove_node(list, cur);
                cur = nxt;
                /* continua para remover duplicatas; corte após encontrar
                   um nó maior (que não pode ser igual) */
                continue;
            }
            /* corte: em percurso crescente, nó maior que key → impossível */
            if (cur->key > key) break;
            cur = cur->next;
        }
    } else {
        Node *cur = list->tail;
        while (cur) {
            nep++;
            if (cur->key == key) {
                *encontrado = 1;
                Node *prv = cur->prev;
                remove_node(list, cur);
                cur = prv;
                continue;
            }
            /* corte: em percurso decrescente, nó menor que key → impossível */
            if (cur->key < key) break;
            cur = cur->prev;
        }
    }

    return nep;
}

/* ── Impressão da tabela ─────────────────────────────────────── */

void print_table(List *table, int m) {
    for (int i = 0; i < m; i++) {
        if (!table[i].head) {
            printf("%d :: NULL\n", i);
            continue;
        }
        /* sentido direto */
        printf("%d :: ", i);
        for (Node *n = table[i].head; n; n = n->next)
            printf("%d -> ", n->key);
        printf("NULL :: ");
        /* sentido reverso */
        for (Node *n = table[i].tail; n; n = n->prev)
            printf("%d -> ", n->key);
        printf("NULL\n");
    }
}

/* ── Main ────────────────────────────────────────────────────── */

int main(void) {
    int m;
    scanf("%d", &m);

    List *table = calloc(m, sizeof(List)); /* heads e tails já NULL */

    int key;
    while (scanf("%d", &key) == 1 && key != -1)
        insert(&table[key % m], key);

    int search;
    scanf("%d", &search);

    int idx = search % m;
    int sentido, encontrado;
    int nep = search_and_remove(&table[idx], search, &sentido, &encontrado);

    /* primeira linha: NEP e sentido */
    if (sentido == 0)
        printf("NEP: 0 (Lista Vazia)\n");
    else if (sentido == 1)
        printf("NEP: %d (Sentido: Inicio)\n", nep);
    else
        printf("NEP: %d (Sentido: Fim)\n", nep);

    if (!encontrado)
        printf("Valor nao encontrado\n");

    print_table(table, m);

    /* libera memória */
    for (int i = 0; i < m; i++) {
        Node *cur = table[i].head;
        while (cur) { Node *nxt = cur->next; free(cur); cur = nxt; }
    }
    free(table);

    return 0;
}
