/**
 * @file priority_queue.c
 * @brief Implementation of a generic Priority Queue using a ternary heap and a hash map for index tracking.
 */

#include "../include/hash_table.h"
#include "../include/priority_queue.h"
#include <stdlib.h>
#include <stdio.h>

#define PQ_INITIAL_CAPACITY 32

/**
 * @brief Priority Queue structure using a ternary heap.
 */
struct PriorityQueue {
    void** heap;            /**< Array ternary heap */
    size_t capacity;        /**< Maximum number of elements before resizing */
    size_t size;            /**< Current number of elements */
    HashTable* index_map;   /**< Map to store element indices for O(1) */
    CompareFn compare;      /**< Function to compare elements */
    HashFn hash;            /**< Function to hash elements */
};

// --- Helpers for Ternary Heap ---

/** @brief Returns parent index of a node. */
static size_t parent_idx(size_t i) { return (i - 1) / 3; }
/** @brief Returns first child index. */
static size_t child_1(size_t i) { return 3 * i + 1; }
/** @brief Returns second child index. */
static size_t child_2(size_t i) { return 3 * i + 2; }
/** @brief Returns third child index. */
static size_t child_3(size_t i) { return 3 * i + 3; }

// --- Map Management ---

/**
 * @brief Updates the index of an element in the internal hash table.
 */
static void update_map(PriorityQueue* pq, void* data, int new_idx) {
    int* ptr = (int*)hash_table_get(pq->index_map, data);

    if (ptr == NULL) {
        ptr = malloc(sizeof(int));
        if (!ptr) return;
        hash_table_put(pq->index_map, data, ptr);
    }
    *ptr = new_idx;
}

/**
 * @brief Removes an element's index tracking from the hash table.
 */
static void remove_from_map(PriorityQueue* pq, void* data) {
    int* ptr = (int*)hash_table_get(pq->index_map, data);
    if (ptr) {
        free(ptr);
        hash_table_remove(pq->index_map, data);
    }
}

// --- Heap Logic ---

/**
 * @brief Swaps two elements in the heap and updates their mapped indices.
 */
static void swap(PriorityQueue* pq, size_t i, size_t j) {
    void* temp = pq->heap[i];
    pq->heap[i] = pq->heap[j];
    pq->heap[j] = temp;

    update_map(pq, pq->heap[i], (int)i);
    update_map(pq, pq->heap[j], (int)j);
}

/**
 * @brief Restores heap property by moving an element up.
 */
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

/**
 * @brief Restores heap property by moving an element down.
 */
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

/**
 * @brief Creates a new Priority Queue.
 * @return Pointer to the new queue or NULL.
 */
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

/**
 * @brief Inserts an element into the queue.
 * @return 1 on success, 0 on failure or if duplicate.
 */
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

/**
 * @brief Returns the element with the highest priority.
 */
void* priority_queue_top(const PriorityQueue* pq) {
    if (!pq || pq->size == 0) return NULL;
    return pq->heap[0];
}

/**
 * @brief Removes the highest priority element.
 */
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

/**
 * @brief Checks if an element exists in the queue.
 */
int priority_queue_contains(const PriorityQueue* pq, const void* data) {
    if (!pq || !data) return 0;
    return hash_table_contains_key(pq->index_map, data);
}

/**
 * @brief Removes an arbitrary element from the queue.
 * @return 1 on success, 0 if not found.
 */
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

/**
 * @brief Returns the current number of elements.
 */
int priority_queue_size(const PriorityQueue* pq) {
    return pq ? (int)pq->size : 0;
}

/**
 * @brief Frees all internal resources of the Priority Queue.
 * @note Does not free the user data pointers.
 */
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