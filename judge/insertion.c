#include <stdio.h>
#include <stdlib.h>

/*
ATV-INSERTION SORT
Guilherme Bento Ramos
185226
N
*/

typedef struct Node {
  int val;
  struct Node *next;
  struct Node *prev;
} Node;

Node *new_Node() {
  Node *new = malloc(sizeof(Node));
  if (new == NULL) {
    exit(1);
  }
  new->val = 0;
  new->next = NULL;
  new->prev = NULL;
  return new;
}

int insert(Node **head, Node **tail, int id) {
  int nep = 0;

  // lista vazia
  if (*head == NULL) {
    Node *new = new_Node();
    new->val = id;
    *head = new;
    *tail = new;
    return 0;
  }

  // insere no inicio
  if (id <= (*head)->val) {
    Node *new = new_Node();
    new->val = id;
    new->next = *head;
    (*head)->prev = new;
    *head = new;
    return 0;
  }

  // insere no fim
  if (id >= (*tail)->val) {
    Node *new = new_Node();
    new->val = id;
    new->prev = *tail;
    (*tail)->next = new;
    *tail = new;
    return 0;
  }

  // busca por proximidade
  int dist_head = abs((*head)->val - id);
  int dist_tail = abs((*tail)->val - id);

  if (dist_head <= dist_tail) {
    // busca a partir do inicio
    Node *curr = (*head)->next;
    nep = 1;
    while (id > curr->val) {
      curr = curr->next;
      nep++;
    }
    Node *new = new_Node();
    new->val = id;
    new->next = curr;
    new->prev = curr->prev;
    curr->prev->next = new;
    curr->prev = new;
  } else {
    // busca a partir do fim
    Node *curr = (*tail)->prev;
    nep = 1;
    while (id < curr->val) {
      curr = curr->prev;
      nep++;
    }
    Node *new = new_Node();
    new->val = id;
    new->prev = curr;
    new->next = curr->next;
    curr->next->prev = new;
    curr->next = new;
  }
  return nep;
}

int main() {
  int n;
  if (scanf("%d", &n) != 1) return 0;

  Node *head = NULL;
  Node *tail = NULL;
  int total_nep = 0;

  for (int i = 0; i < n; i++) {
    int id;
    scanf("%d", &id);
    total_nep += insert(&head, &tail, id);
  }

  // imprime lista ordenada
  Node *curr = head;
  while (curr) {
    printf("%d%c", curr->val, curr->next ? ' ' : '\n');
    curr = curr->next;
  }
  printf("%d\n", total_nep);

  return 0;
}
