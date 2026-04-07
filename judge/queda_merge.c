#include <stdio.h>
#include <stdlib.h>

/*
  AED2
  Guilherme Bento Ramos 185226
  AP02 - Merge Sort (Contagem de Inversões Significativas)
*/

long long count_significant_inversions(int arr[], int left, int mid, int right) {
    long long count = 0;
    int j = mid + 1;
    for (int i = left; i <= mid; i++) {
        while (j <= right && (long long)arr[i] > 2LL * arr[j]) {
            j++;
        }
        count += (j - (mid + 1));
    }
    return count;
}

void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int i = 0; i < n2; i++) R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    free(L);
    free(R);
}

long long merge_sort(int arr[], int left, int right) {
    long long count = 0;
    if (left < right) {
        int mid = left + (right - left) / 2;
        count += merge_sort(arr, left, mid);
        count += merge_sort(arr, mid + 1, right);
        count += count_significant_inversions(arr, left, mid, right);
        merge(arr, left, mid, right);
    }
    return count;
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    if (n <= 0) {
        printf("0\n\n");
        return 0;
    }

    int *nums = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        scanf("%d", &nums[i]);
    }

    long long counter = merge_sort(nums, 0, n - 1);

    printf("%lld\n", counter);

    for (int i = 0; i < n; i++) {
        printf("%d%c", nums[i], (i == n - 1 ? '\n' : ' '));
    }

    free(nums);
    return 0;
}
