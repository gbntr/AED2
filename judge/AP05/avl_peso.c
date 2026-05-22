#include <stdio.h>
#include <stdlib.h>

/*
  AED2
  Guilherme Bento Ramos 185226
  AP05 - Arvore AVL baseada em peso
*/

typedef struct Node {
    int key;
    int weight;
    struct Node *left;
    struct Node *right;
} Node;

static int get_weight(Node *node) {
    return node == NULL ? 0 : node->weight;
}

static void update_weight(Node *node) {
    if (node != NULL) {
        node->weight = 1 + get_weight(node->left) + get_weight(node->right);
    }
}

static Node *new_node(int key) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        exit(1);
    }
    node->key = key;
    node->weight = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static Node *rotate_right(Node *a) {
    Node *b = a->left;
    Node *t2 = b->right;

    b->right = a;
    a->left = t2;

    update_weight(a);
    update_weight(b);
    return b;
}

static Node *rotate_left(Node *a) {
    Node *b = a->right;
    Node *t2 = b->left;

    b->left = a;
    a->right = t2;

    update_weight(a);
    update_weight(b);
    return b;
}

static Node *rebalance(Node *node) {
    int wl = get_weight(node->left);
    int wr = get_weight(node->right);

    if (wl + 1 > 2 * (wr + 1)) {
        Node *b = node->left;
        if (get_weight(b->left) < get_weight(b->right)) {
            node->left = rotate_left(b);
        }
        return rotate_right(node);
    }

    if (wr + 1 > 2 * (wl + 1)) {
        Node *b = node->right;
        if (get_weight(b->right) < get_weight(b->left)) {
            node->right = rotate_right(b);
        }
        return rotate_left(node);
    }

    return node;
}

static Node *insert_node(Node *node, int key) {
    if (node == NULL) {
        return new_node(key);
    }

    if (key < node->key) {
        node->left = insert_node(node->left, key);
    } else if (key > node->key) {
        node->right = insert_node(node->right, key);
    } else {
        return node;
    }

    update_weight(node);
    return rebalance(node);
}

static Node *remove_max(Node *node, Node **max_node) {
    if (node->right == NULL) {
        *max_node = node;
        return node->left;
    }

    node->right = remove_max(node->right, max_node);
    update_weight(node);
    return rebalance(node);
}

static Node *remove_node(Node *node, int key) {
    if (node == NULL) {
        return NULL;
    }

    if (key < node->key) {
        node->left = remove_node(node->left, key);
    } else if (key > node->key) {
        node->right = remove_node(node->right, key);
    } else {
        if (node->left == NULL) {
            Node *right = node->right;
            free(node);
            return right;
        }
        if (node->right == NULL) {
            Node *left = node->left;
            free(node);
            return left;
        }

        Node *pred = NULL;
        node->left = remove_max(node->left, &pred);
        pred->left = node->left;
        pred->right = node->right;
        free(node);
        node = pred;
    }

    update_weight(node);
    return rebalance(node);
}

static void print_range(Node *node, int x, int y, int *first) {
    if (node == NULL) {
        return;
    }

    if (node->key > x) {
        print_range(node->left, x, y, first);
    }

    if (node->key >= x && node->key <= y) {
        if (!(*first)) {
            printf(", ");
        }
        printf("%d", node->key);
        *first = 0;
    }

    if (node->key < y) {
        print_range(node->right, x, y, first);
    }
}

static void free_tree(Node *node) {
    if (node == NULL) {
        return;
    }
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}

int main(void) {
    Node *root = NULL;
    int value;

    while (scanf("%d", &value) == 1 && value != -1) {
        if (value > 0) {
            root = insert_node(root, value);
        }
    }

    if (root == NULL) {
        printf("ARVORE VAZIA\n");
    } else {
        printf("%d,%d,%d\n", root->weight, get_weight(root->left), get_weight(root->right));
    }

    while (scanf("%d", &value) == 1 && value != -1) {
        if (value > 0) {
            root = remove_node(root, value);
        }
    }

    if (root == NULL) {
        printf("ARVORE VAZIA\n");
    } else {
        printf("%d,%d,%d\n", root->weight, get_weight(root->left), get_weight(root->right));
    }

    int x, y;
    if (scanf("%d %d", &x, &y) != 2) {
        free_tree(root);
        return 0;
    }

    int first = 1;
    print_range(root, x, y, &first);
    if (first) {
        printf("NADA A EXIBIR");
    }
    printf("\n");

    free_tree(root);
    return 0;
}
