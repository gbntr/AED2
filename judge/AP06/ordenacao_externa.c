#include <stdio.h>
#include <stdlib.h>

// Estruturas para simular as fitas magnéticas
typedef struct {
    int* elements;
    int size;
    int capacity;
} Block;

typedef struct {
    Block* blocks;
    int num_blocks;
    int capacity;
} Tape;

Tape* tapes;

void init_tape(Tape* t) {
    t->blocks = NULL;
    t->num_blocks = 0;
    t->capacity = 0;
}

void add_block(Tape* t) {
    if (t->num_blocks == t->capacity) {
        t->capacity = t->capacity == 0 ? 4 : t->capacity * 2;
        t->blocks = (Block*)realloc(t->blocks, t->capacity * sizeof(Block));
    }
    t->blocks[t->num_blocks].elements = NULL;
    t->blocks[t->num_blocks].size = 0;
    t->blocks[t->num_blocks].capacity = 0;
    t->num_blocks++;
}

void add_element_to_block(Block* b, int val) {
    if (b->size == b->capacity) {
        b->capacity = b->capacity == 0 ? 16 : b->capacity * 2;
        b->elements = (int*)realloc(b->elements, b->capacity * sizeof(int));
    }
    b->elements[b->size++] = val;
}

// Funções para manipulação do Min-Heap (Fase 1)
void sift_down(int* arr, int i, int size) {
    int min_index = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < size && arr[left] < arr[min_index]) min_index = left;
    if (right < size && arr[right] < arr[min_index]) min_index = right;
    if (min_index != i) {
        int temp = arr[i];
        arr[i] = arr[min_index];
        arr[min_index] = temp;
        sift_down(arr, min_index, size);
    }
}

void build_heap(int* arr, int size) {
    for (int i = size / 2 - 1; i >= 0; i--) {
        sift_down(arr, i, size);
    }
}

// Funções para manipulação do Min-Heap (Fase 2)
typedef struct {
    int val;
    int tape_id;
} HeapNode;

void sift_down_node(HeapNode* arr, int i, int size) {
    int min_index = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < size && arr[left].val < arr[min_index].val) min_index = left;
    if (right < size && arr[right].val < arr[min_index].val) min_index = right;
    if (min_index != i) {
        HeapNode temp = arr[i];
        arr[i] = arr[min_index];
        arr[min_index] = temp;
        sift_down_node(arr, min_index, size);
    }
}

void build_heap_node(HeapNode* arr, int size) {
    for (int i = size / 2 - 1; i >= 0; i--) {
        sift_down_node(arr, i, size);
    }
}

int main() {
    int N, M, K;
    if (scanf("%d %d %d", &N, &M, &K) != 3) return 0;
    
    tapes = (Tape*)malloc(2 * K * sizeof(Tape));
    for (int i = 0; i < 2 * K; i++) {
        init_tape(&tapes[i]);
    }
    
    // Leitura inicial e construção do Heap da Fase 1
    int* arr = (int*)malloc(M * sizeof(int));
    int total_size = 0;
    for (int i = 0; i < M && i < N; i++) {
        scanf("%d", &arr[i]);
        total_size++;
    }
    
    int active_size = total_size;
    build_heap(arr, active_size);
    
    printf("[Fase 1 - Heap Inicial]: ");
    for(int i = 0; i < active_size; i++) {
        printf("%d%s", arr[i], i == active_size - 1 ? "" : " ");
    }
    printf("\n");
    
    int elements_read_so_far = total_size;
    int current_tape = 0;
    int first_block_done = 0;
    
    // Geração de Blocos (Seleção por Substituição)
    while (total_size > 0) {
        if (active_size == 0) {
            active_size = total_size;
            build_heap(arr, active_size);
            current_tape = (current_tape + 1) % K;
        }
        
        add_block(&tapes[current_tape]);
        Block* current_block = &tapes[current_tape].blocks[tapes[current_tape].num_blocks - 1];
        
        while (active_size > 0) {
            int extracted = arr[0];
            add_element_to_block(current_block, extracted);
            
            if (elements_read_so_far < N) {
                int x;
                scanf("%d", &x);
                elements_read_so_far++;
                if (x >= extracted) {
                    arr[0] = x;
                    sift_down(arr, 0, active_size);
                } else {
                    arr[0] = arr[active_size - 1];
                    arr[active_size - 1] = x;
                    active_size--;
                    sift_down(arr, 0, active_size);
                }
            } else {
                // Fim do arquivo, reduz o tamanho do array
                arr[0] = arr[active_size - 1];
                arr[active_size - 1] = arr[total_size - 1];
                total_size--;
                active_size--;
                sift_down(arr, 0, active_size);
            }
            
            // Impressão do Heap Alterado após o primeiro bloco
            if (active_size == 0 && total_size > 0 && !first_block_done) {
                printf("[Fase 1 - Heap Alterado]: ");
                for(int i = 0; i < total_size; i++) {
                    printf("%d%s", arr[i], i == total_size - 1 ? "" : " ");
                }
                printf("\n");
                first_block_done = 1;
            }
        }
    }
    
    printf("[Fase 1 - Fitas Geradas]:\n");
    for (int i = 0; i < K; i++) {
        printf("Fita %d:", i);
        for (int b = 0; b < tapes[i].num_blocks; b++) {
            printf(" [");
            for (int e = 0; e < tapes[i].blocks[b].size; e++) {
                printf("%d%s", tapes[i].blocks[b].elements[e], e == tapes[i].blocks[b].size - 1 ? "" : " ");
            }
            printf("]");
        }
        printf("\n");
    }
    
    // Fase 2: Intercalação Balanceada (K-Way)
    int input_start = 0;
    int output_start = K;
    
    int is_first_phase2_merge = 1;
    int is_first_pass = 1;
    
    HeapNode* heap = (HeapNode*)malloc(K * sizeof(HeapNode));
    int* tape_block_idx = (int*)malloc(K * sizeof(int));
    int* block_elem_idx = (int*)malloc(K * sizeof(int));
    
    while (1) {
        int out_tape_idx = output_start;
        int blocks_generated_in_pass = 0;
        
        for(int i = 0; i < K; i++) tape_block_idx[i] = 0;
        
        while (1) {
            int has_blocks = 0;
            for(int i = 0; i < K; i++) {
                if (tape_block_idx[i] < tapes[input_start + i].num_blocks) {
                    has_blocks = 1;
                    break;
                }
            }
            if (!has_blocks) break;
            
            int heap_size = 0;
            for(int i = 0; i < K; i++) block_elem_idx[i] = 0;
            
            for(int i = 0; i < K; i++) {
                if (tape_block_idx[i] < tapes[input_start + i].num_blocks) {
                    int tape_actual = input_start + i;
                    Block* b = &tapes[tape_actual].blocks[tape_block_idx[i]];
                    heap[heap_size].val = b->elements[0];
                    heap[heap_size].tape_id = i;
                    block_elem_idx[i] = 1;
                    heap_size++;
                }
            }
            
            build_heap_node(heap, heap_size);
            
            if (is_first_phase2_merge) {
                printf("[Fase 2 - Heap Intercalacao]: ");
                for(int i = 0; i < heap_size; i++) {
                    printf("%d%s", heap[i].val, i == heap_size - 1 ? "" : " ");
                }
                printf("\n");
                is_first_phase2_merge = 0;
            }
            
            add_block(&tapes[out_tape_idx]);
            Block* out_block = &tapes[out_tape_idx].blocks[tapes[out_tape_idx].num_blocks - 1];
            
            while (heap_size > 0) {
                HeapNode min_node = heap[0];
                add_element_to_block(out_block, min_node.val);
                
                int t_id = min_node.tape_id;
                int tape_actual = input_start + t_id;
                Block* b = &tapes[tape_actual].blocks[tape_block_idx[t_id]];
                
                if (block_elem_idx[t_id] < b->size) {
                    heap[0].val = b->elements[block_elem_idx[t_id]++];
                    sift_down_node(heap, 0, heap_size);
                } else {
                    heap[0] = heap[heap_size - 1];
                    heap_size--;
                    sift_down_node(heap, 0, heap_size);
                    tape_block_idx[t_id]++;
                }
            }
            
            out_tape_idx = output_start + ((out_tape_idx - output_start + 1) % K);
            blocks_generated_in_pass++;
        }
        
        if (is_first_pass) {
            printf("[Fase 2 - Fitas apos 1a Passada]:\n");
            for(int i = 0; i < K; i++) {
                int actual_tape = output_start + i;
                printf("Fita %d:", actual_tape);
                for(int b = 0; b < tapes[actual_tape].num_blocks; b++) {
                    printf(" [");
                    for(int e = 0; e < tapes[actual_tape].blocks[b].size; e++) {
                        printf("%d%s", tapes[actual_tape].blocks[b].elements[e], e == tapes[actual_tape].blocks[b].size - 1 ? "" : " ");
                    }
                    printf("]");
                }
                printf("\n");
            }
            is_first_pass = 0;
        }
        
        if (blocks_generated_in_pass <= 1) {
            printf("[Resultado Final]: ");
            for(int i = 0; i < K; i++) {
                int actual_tape = output_start + i;
                if (tapes[actual_tape].num_blocks > 0) {
                    Block* b = &tapes[actual_tape].blocks[0];
                    for(int e = 0; e < b->size; e++) {
                        printf("%d%s", b->elements[e], e == b->size - 1 ? "" : " ");
                    }
                    break;
                }
            }
            printf("\n");
            break;
        }
        
        for(int i = 0; i < K; i++) {
            tapes[input_start + i].num_blocks = 0;
        }
        
        int temp = input_start;
        input_start = output_start;
        output_start = temp;
    }
    
    // Liberação de memória pode ser omitida em juízes online
    free(heap);
    free(tape_block_idx);
    free(block_elem_idx);
    free(arr);
    
    return 0;
}
