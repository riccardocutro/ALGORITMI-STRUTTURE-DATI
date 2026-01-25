#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stddef.h>

// structure
typedef struct PriorityQueue PriorityQueue;

// Function pointers
// Returns >0 if a has higher priority than b
typedef int (*CompareFn)(const void* a, const void* b);
// Returns hash of the key (for the map)
typedef unsigned long (*HashFn)(const void* key);

// Creates a new ternary heap
PriorityQueue* priority_queue_create(CompareFn f1, HashFn f2);

// Adds an element. Returns 1 on success, 0 on error.
int priority_queue_push(PriorityQueue* pq, void* data);

// Returns the element with max priority
void* priority_queue_top(const PriorityQueue* pq);

// Removes the element with max priority
void priority_queue_pop(PriorityQueue* pq);

// Checks if data is in the queue. Complexity: O(1) thanks to Hash Map.
int priority_queue_contains(const PriorityQueue* pq, const void* data);

// Removes a specific element. Complexity: O(log N).
int priority_queue_remove(PriorityQueue* pq, const void* data);

// Returns number of elements
int priority_queue_size(const PriorityQueue* pq);

// Frees memory
void priority_queue_free(PriorityQueue* pq);

#endif