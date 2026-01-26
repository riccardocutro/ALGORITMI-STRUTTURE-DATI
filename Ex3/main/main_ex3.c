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

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input_csv> <output_csv>\n", argv[0]);
        return 1;
    }

    FILE* fin = fopen(argv[1], "r");
    if (!fin) { perror("Error opening input"); return 1; }

    size_t capacity = INITIAL_CAPACITY;
    Task** all_tasks = malloc(sizeof(Task*) * capacity);
    if (!all_tasks) { perror("Malloc failed"); fclose(fin); return 1; }

    int count = 0;
    char line[MAX_LINE];

    while (fgets(line, MAX_LINE, fin)) {
        if (count >= (int)capacity) {
            capacity *= 2;
            Task** temp = realloc(all_tasks, sizeof(Task*) * capacity);
            if (!temp) {
                perror("Realloc failed");
                return 1;
            }
            all_tasks = temp;
        }

        Task* t = malloc(sizeof(Task));
        if (sscanf(line, "%d,%d,%d,%d", &t->id, &t->start, &t->length, &t->priority) == 4) {
            all_tasks[count++] = t;
        } else {
            free(t);
        }
    }
    fclose(fin);

    qsort(all_tasks, count, sizeof(Task*), compare_start_time);

    PriorityQueue* pq = priority_queue_create(compare_tasks, hash_task);
    FILE* fout = fopen(argv[2], "w");
    if (!fout) { perror("Error opening output"); return 1; }

    int current_time = 0;
    int task_idx = 0;
    int completed = 0;

    printf("Starting simulation with %d tasks...\n", count);

    while (completed < count) {
        while (task_idx < count && all_tasks[task_idx]->start <= current_time) {
            priority_queue_push(pq, all_tasks[task_idx]);
            task_idx++;
        }
        if (priority_queue_size(pq) > 0) {
            Task* t = (Task*)priority_queue_top(pq);
            priority_queue_pop(pq);

            if (current_time < t->start) {
                 current_time = t->start;
            }
            int start_exec = current_time;
            int end_exec = current_time + t->length;
            fprintf(fout, "%d,%d,%d\n", t->id, start_exec, end_exec);
            current_time = end_exec;
            completed++;
        } else {
            if (task_idx < count) {
                current_time = all_tasks[task_idx]->start;
            } else {
                break;
            }
        }
    }
    printf("Simulation finished. Output saved to %s\n", argv[2]);
    fclose(fout);
    priority_queue_free(pq);
    for(int i=0; i<count; i++) free(all_tasks[i]);
    free(all_tasks);

    return 0;
}