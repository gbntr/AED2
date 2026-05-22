#include <stdio.h>
#include <stdlib.h>

/*
  AED2
  Guilherme Bento Ramos 185226
  AP03 - Quicksort (Median of Three)
*/

typedef struct {
    int id;
    int gravity;
} Patient;

long long swap_count = 0;

/**
 * Realiza a troca de dois pacientes se os índices forem diferentes
 * e o conteúdo (ID ou Gravidade) for diferente.
 */
void swap(Patient *a, Patient *b, int idx_a, int idx_b) {
    if (idx_a == idx_b) return;
    if (a->id == b->id && a->gravity == b->gravity) return;

    Patient temp = *a;
    *a = *b;
    *b = temp;
    swap_count++;
}

/**
 * Retorna verdadeiro se p1 tem maior prioridade que p2.
 * Prioridade: Maior gravidade primeiro; se igual, menor ID primeiro.
 */
int is_higher_priority(Patient p1, Patient p2) {
    if (p1.gravity > p2.gravity) return 1;
    if (p1.gravity < p2.gravity) return 0;
    return p1.id < p2.id;
}

int partition(Patient arr[], int low, int high) {
    int mid = low + (high - low) / 2;
    
    // Mediana de três: ordena low, mid e high para escolher o pivô
    if (is_higher_priority(arr[mid], arr[low])) swap(&arr[low], &arr[mid], low, mid);
    if (is_higher_priority(arr[high], arr[low])) swap(&arr[low], &arr[high], low, high);
    if (is_higher_priority(arr[high], arr[mid])) swap(&arr[mid], &arr[high], mid, high);
    
    // O pivô é a mediana (arr[mid]). Move para a posição high para particionar.
    swap(&arr[mid], &arr[high], mid, high);
    
    Patient pivot = arr[high];
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        if (is_higher_priority(arr[j], pivot)) {
            i++;
            swap(&arr[i], &arr[j], i, j);
        }
    }
    swap(&arr[i + 1], &arr[high], i + 1, high);
    return i + 1;
}

void quick_sort(Patient arr[], int low, int high) {
    if (low < high) {
        int p = partition(arr, low, high);
        quick_sort(arr, low, p - 1);
        quick_sort(arr, p + 1, high);
    }
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    
    if (n <= 0) {
        printf("Swaps: 0\n");
        return 0;
    }
    
    Patient *patients = (Patient *)malloc(n * sizeof(Patient));
    if (patients == NULL) return 1;

    for (int i = 0; i < n; i++) {
        if (scanf("%d %d", &patients[i].id, &patients[i].gravity) != 2) break;
    }
    
    quick_sort(patients, 0, n - 1);
    
    for (int i = 0; i < n; i++) {
        printf("%d\n", patients[i].id);
    }
    printf("Swaps: %lld\n", swap_count);
    
    free(patients);
    return 0;
}
