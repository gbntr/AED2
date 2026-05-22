#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  AED2 2026 (1S) - AP04
  HeapSort com filtragem por frequencia global de caracteres.
*/

typedef struct {
    char text[21];
    int weight;
} Word;

static int is_greater(const Word *a, const Word *b) {
    if (a->weight != b->weight) {
        return a->weight > b->weight;
    }
    return strcmp(a->text, b->text) > 0;
}

static void swap_words(Word *a, Word *b) {
    if (a == b) return;
    Word temp = *a;
    *a = *b;
    *b = temp;
}

static void heapify(Word arr[], int size, int root) {
    while (1) {
        int largest = root;
        int left = 2 * root + 1;
        int right = 2 * root + 2;

        if (left < size && is_greater(&arr[left], &arr[largest])) {
            largest = left;
        }
        if (right < size && is_greater(&arr[right], &arr[largest])) {
            largest = right;
        }

        if (largest == root) {
            break;
        }

        swap_words(&arr[root], &arr[largest]);
        root = largest;
    }
}

static void build_heap(Word arr[], int size) {
    for (int i = (size / 2) - 1; i >= 0; i--) {
        heapify(arr, size, i);
    }
}

int main(void) {
    int n, k;
    if (scanf("%d %d", &n, &k) != 2) {
        return 0;
    }

    Word *words = (Word *)malloc((size_t)n * sizeof(Word));
    if (words == NULL) {
        return 1;
    }

    int freq[26] = {0};

    for (int i = 0; i < n; i++) {
        if (scanf("%20s", words[i].text) != 1) {
            free(words);
            return 0;
        }
        words[i].weight = 0;
        for (int j = 0; words[i].text[j] != '\0'; j++) {
            freq[words[i].text[j] - 'a']++;
        }
    }

    int valid_count = 0;
    for (int i = 0; i < n; i++) {
        int valid = 1;
        int weight = 0;

        for (int j = 0; words[i].text[j] != '\0'; j++) {
            int idx = words[i].text[j] - 'a';
            if (freq[idx] < k) {
                valid = 0;
                break;
            }
            weight += freq[idx];
        }

        if (!valid) {
            printf("A palavra %s eh invalida (K=%d)\n", words[i].text, k);
            continue;
        }

        words[i].weight = weight;
        if (valid_count != i) {
            words[valid_count] = words[i];
        }
        valid_count++;
    }

    if (valid_count == 0) {
        printf("Erro: Sem palavras validas para ordenacao.\n");
        free(words);
        return 0;
    }

    build_heap(words, valid_count);
    printf("build_heap:");
    for (int i = 0; i < valid_count; i++) {
        printf(" %s", words[i].text);
    }
    printf("\n");

    for (int end = valid_count - 1; end > 0; end--) {
        swap_words(&words[0], &words[end]);
        heapify(words, end, 0);
    }

    printf("palavras:");
    for (int i = 0; i < valid_count; i++) {
        printf(" %s", words[i].text);
    }
    printf("\n");

    free(words);
    return 0;
}
