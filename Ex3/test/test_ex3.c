#include "unity.h"
#include "priority_queue.h"
#include <stdlib.h>

PriorityQueue* pq;

int compare_int(const void* a, const void* b) {
    return (*(int*)a) - (*(int*)b);
}

unsigned long hash_int(const void* key) {
    return *(unsigned long*)key;
}

