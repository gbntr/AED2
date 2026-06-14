/*
 * AED2 2026 (1S) - AP-07
 * Arvore B+: insercao e pesquisa. Ordem m lida em tempo de execucao.
 * Folhas: mediano (idx m) duplicado na nova folha direita.
 * Internos: mediano promovido e removido do no original.
 */

#include <stdio.h>
#include <stdlib.h>

int m;

typedef struct Node {
    int *keys;
    int n;
    struct Node **children;
    struct Node *next;  /* lista encadeada de folhas */
    int is_leaf;
} Node;

typedef struct {
    int key;
    Node *right;
} Split;

static Node *make_node(int is_leaf) {
    Node *nd = malloc(sizeof(Node));
    nd->keys = malloc((2*m + 2) * sizeof(int));
    nd->n = 0;
    nd->is_leaf = is_leaf;
    nd->next = NULL;
    nd->children = is_leaf ? NULL : malloc((2*m + 3) * sizeof(Node*));
    return nd;
}

/* Retorna 1 se houve split, preenchendo sp com chave promovida e filho direito */
static int insert_rec(Node *nd, int key, Split *sp) {
    if (nd->is_leaf) {
        int i = nd->n - 1;
        while (i >= 0 && nd->keys[i] > key) {
            nd->keys[i+1] = nd->keys[i];
            i--;
        }
        nd->keys[i+1] = key;
        nd->n++;

        if (nd->n > 2*m) {
            Node *right = make_node(1);
            sp->key = nd->keys[m];           /* mediano duplicado */
            right->n = nd->n - m;
            for (int j = 0; j < right->n; j++)
                right->keys[j] = nd->keys[m + j];
            nd->n = m;
            right->next = nd->next;
            nd->next = right;
            sp->right = right;
            return 1;
        }
        return 0;
    }

    /* No interno: desce para filho correto */
    int i = nd->n - 1;
    while (i >= 0 && key < nd->keys[i]) i--;

    Split child_sp;
    if (insert_rec(nd->children[i+1], key, &child_sp)) {
        int pos = nd->n - 1;
        while (pos >= 0 && nd->keys[pos] > child_sp.key) {
            nd->keys[pos+1] = nd->keys[pos];
            nd->children[pos+2] = nd->children[pos+1];
            pos--;
        }
        nd->keys[pos+1] = child_sp.key;
        nd->children[pos+2] = child_sp.right;
        nd->n++;

        if (nd->n > 2*m) {
            Node *right = make_node(0);
            sp->key = nd->keys[m];           /* mediano promovido e removido */
            right->n = nd->n - m - 1;
            for (int j = 0; j < right->n; j++)
                right->keys[j] = nd->keys[m + 1 + j];
            for (int j = 0; j <= right->n; j++)
                right->children[j] = nd->children[m + 1 + j];
            nd->n = m;
            sp->right = right;
            return 1;
        }
    }
    return 0;
}

Node *root;

static void insert(int key) {
    Split sp;
    if (insert_rec(root, key, &sp)) {
        Node *new_root = make_node(0);
        new_root->keys[0] = sp.key;
        new_root->children[0] = root;
        new_root->children[1] = sp.right;
        new_root->n = 1;
        root = new_root;
    }
}

static void search(int key) {
    Node *nd = root;
    int pages = 0;

    while (!nd->is_leaf) {
        pages++;
        int i = 0;
        while (i < nd->n && key >= nd->keys[i]) i++;
        nd = nd->children[i];
    }
    pages++;  /* conta a folha */

    printf("%d\n", pages);

    int found = 0;
    for (int i = 0; i < nd->n; i++)
        if (nd->keys[i] == key) { found = 1; break; }

    if (found) {
        for (int i = 0; i < nd->n; i++) {
            if (i > 0) printf(" ");
            printf("%d", nd->keys[i]);
        }
        printf("\n");
    } else {
        printf("Valor nao encontrado\n");
    }
}

int main(void) {
    scanf("%d", &m);
    root = make_node(1);

    int key;
    while (scanf("%d", &key) == 1 && key > 0)
        insert(key);

    printf("%d\n", root->n);

    scanf("%d", &key);
    search(key);

    return 0;
}
