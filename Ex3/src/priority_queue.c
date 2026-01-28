#include "../include/hash_table.h"
#include "/priority_queue.h"
#include <stdlib.h>
#include <stdio.h>

#define PQ_INITIAL_CAPACITY 32

struct PriorityQueue {
    void** heap;
    size_t capacity;
    size_t size;
    HashTable* index_map;
    CompareFn compare;
    HashFn hash;
};

// --- Helpers for Ternary Heap ---
static size_t parent_idx(size_t i) { return (i - 1) / 3; }
static size_t child_1(size_t i) { return 3 * i + 1; }
static size_t child_2(size_t i) { return 3 * i + 2; }
static size_t child_3(size_t i) { return 3 * i + 3; }

// --- Map Management ---

static void update_map(PriorityQueue* pq, void* data, int new_idx) {
    int* ptr = (int*)hash_table_get(pq->index_map, data);

    if (ptr == NULL) {
        ptr = malloc(sizeof(int));
        if (!ptr) return;
        hash_table_put(pq->index_map, data, ptr);
    }
    *ptr = new_idx;
}

// Removes an element from the Hash Table
static void remove_from_map(PriorityQueue* pq, void* data) {
    int* ptr = (int*)hash_table_get(pq->index_map, data);
    if (ptr) {
        free(ptr);
        hash_table_remove(pq->index_map, data);
    }
}

// --- Heap Logic ---

static void swap(PriorityQueue* pq, size_t i, size_t j) {
    void* temp = pq->heap[i];
    pq->heap[i] = pq->heap[j];
    pq->heap[j] = temp;

    update_map(pq, pq->heap[i], (int)i);
    update_map(pq, pq->heap[j], (int)j);
}

// Moves element up
static void heapify_up(PriorityQueue* pq, size_t idx) {
    while (idx > 0) {
        size_t p = parent_idx(idx);
        if (pq->compare(pq->heap[idx], pq->heap[p]) > 0) {
            swap(pq, idx, p);
            idx = p;
        } else {
            break;
        }
    }
}

// Moves element down
static void heapify_down(PriorityQueue* pq, size_t idx) {
    while (1) {
        size_t max = idx;
        size_t c1 = child_1(idx);
        size_t c2 = child_2(idx);
        size_t c3 = child_3(idx);

        if (c1 < pq->size && pq->compare(pq->heap[c1], pq->heap[max]) > 0) max = c1;
        if (c2 < pq->size && pq->compare(pq->heap[c2], pq->heap[max]) > 0) max = c2;
        if (c3 < pq->size && pq->compare(pq->heap[c3], pq->heap[max]) > 0) max = c3;

        if (max != idx) {
            swap(pq, idx, max);
            idx = max;
        } else {
            break;
        }
    }
}

// --- Implementation ---

PriorityQueue* priority_queue_create(CompareFn f1, HashFn f2) {
    if (!f1 || !f2) return NULL;

    PriorityQueue* pq = malloc(sizeof(PriorityQueue));
    if (!pq) return NULL;

    pq->capacity = PQ_INITIAL_CAPACITY;
    pq->size = 0;
    pq->compare = f1;
    pq->hash = f2;
    pq->heap = malloc(pq->capacity * sizeof(void*));
    pq->index_map = hash_table_create(f1, f2);

    if (!pq->heap || !pq->index_map) {
        if (pq->heap) free(pq->heap);
        if (pq->index_map) hash_table_free(pq->index_map);
        free(pq);
        return NULL;
    }
    return pq;
}

int priority_queue_push(PriorityQueue* pq, void* data) {
    if (!pq || !data) return 0;
    if (priority_queue_contains(pq, data)) return 0;

    if (pq->size == pq->capacity) {
        size_t new_cap = pq->capacity * 2;
        void** new_heap = realloc(pq->heap, new_cap * sizeof(void*));
        if (!new_heap) return 0;
        pq->heap = new_heap;
        pq->capacity = new_cap;
    }

    size_t idx = pq->size;
    pq->heap[idx] = data;
    update_map(pq, data, (int)idx);
    pq->size++;
    heapify_up(pq, idx);
    return 1;
}

void* priority_queue_top(const PriorityQueue* pq) {
    if (!pq || pq->size == 0) return NULL;
    return pq->heap[0];
}

void priority_queue_pop(PriorityQueue* pq) {
    if (!pq || pq->size == 0) return;

    remove_from_map(pq, pq->heap[0]);

    size_t last = pq->size - 1;
    if (pq->size > 1) {
        pq->heap[0] = pq->heap[last];
        update_map(pq, pq->heap[0], 0);
        pq->size--;
        heapify_down(pq, 0);
    } else {
        pq->size--;
    }
}

int priority_queue_contains(const PriorityQueue* pq, const void* data) {
    if (!pq || !data) return 0;
    return hash_table_contains_key(pq->index_map, data);
}

int priority_queue_remove(PriorityQueue* pq, const void* data) {
    if (!pq || !data) return 0;

    int* ptr = (int*)hash_table_get(pq->index_map, data);
    if (!ptr) return 0;

    size_t idx = (size_t)*ptr;
    remove_from_map(pq, (void*)data);

    if (idx == pq->size - 1) {
        pq->size--;
        return 1;
    }

    size_t last = pq->size - 1;
    void* moved = pq->heap[last];
    pq->heap[idx] = moved;
    update_map(pq, moved, (int)idx);
    pq->size--;

    size_t p = parent_idx(idx);
    if (idx > 0 && pq->compare(pq->heap[idx], pq->heap[p]) > 0) {
        heapify_up(pq, idx);
    } else {
        heapify_down(pq, idx);
    }
    return 1;
}

int priority_queue_size(const PriorityQueue* pq) {
    return pq ? (int)pq->size : 0;
}

void priority_queue_free(PriorityQueue* pq) {
    if (!pq) return;

    void** keys = hash_table_keyset(pq->index_map);
    int count = hash_table_size(pq->index_map);

    if (keys) {
        for(int i=0; i<count; i++) {
            int* val = (int*)hash_table_get(pq->index_map, keys[i]);
            free(val);
        }
        free(keys);
    }

    hash_table_free(pq->index_map);
    free(pq->heap);
    free(pq);
}