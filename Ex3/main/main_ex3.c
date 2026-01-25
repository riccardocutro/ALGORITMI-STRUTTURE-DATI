#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "priority_queue.h"

#define MAX_LINE 1024
#define INITIAL_CAPACITY 1024

typedef struct {
    int id;
    int start;
    int length;
    int priority;
} Task;

// Compare tasks for PriorityQueue (Higher priority wins)
int compare_tasks(const void* a, const void* b) {
    Task* t1 = (Task*)a;
    Task* t2 = (Task*)b;
    return t1->priority - t2->priority;
}

// Hash function for tasks (using ID)
unsigned long hash_task(const void* key) {
    Task* t = (Task*)key;
    return (unsigned long)t->id;
}

// Compare for qsort (sorting by Start time)
int compare_start_time(const void* a, const void* b) {
    Task* t1 = *(Task**)a;
    Task* t2 = *(Task**)b;
    return t1->start - t2->start;
}
