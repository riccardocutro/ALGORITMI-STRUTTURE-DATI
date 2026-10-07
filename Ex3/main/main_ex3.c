/**
 * @file main_ex3.c
 * @brief Task scheduling simulation using a Priority Queue.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "priority_queue.h"

#define MAX_LINE 1024
#define INITIAL_CAPACITY 1024

/**
 * @brief Represents a task to be scheduled.
 */
typedef struct {
    int id;       /**< Unique task identifier */
    int start;    /**< Arrival time of the task */
    int length;   /**< Execution duration */
    int priority; /**< Priority level */
} Task;

/**
 * @brief Comparator for Priority Queue based on task priority.
 * @return Positive if t1 > t2, negative if t1 < t2, 0 if equal.
 */
int compare_tasks(const void* a, const void* b) {
    Task* t1 = (Task*)a;
    Task* t2 = (Task*)b;
    return t1->priority - t2->priority;
}

/**
 * @brief Hash function for tasks using their ID.
 */
unsigned long hash_task(const void* key) {
    Task* t = (Task*)key;
    return (unsigned long)t->id;
}

/**
 * @brief Comparator for qsort to sort tasks by arrival time.
 */
int compare_start_time(const void* a, const void* b) {
    Task* t1 = *(Task**)a;
    Task* t2 = *(Task**)b;
    return t1->start - t2->start;
}

/**
 * @brief Main entry point: simulates task scheduling from CSV input to CSV output.
 * @return 0 on success, 1 on error.
 */
int main(int argc, char* argv[]) {
    // Argument validation
    if (argc != 3) {
        printf("Usage: %s <input_csv> <output_csv>\n", argv[0]);
        return 1;
    }

    // Input file opening
    FILE* fin = fopen(argv[1], "r");
    if (!fin) { perror("Error opening input"); return 1; }

    // Dynamic array for task storage
    size_t capacity = INITIAL_CAPACITY;
    Task** all_tasks = malloc(sizeof(Task*) * capacity);
    if (!all_tasks) { perror("Malloc failed"); fclose(fin); return 1; }

    int count = 0;
    char line[MAX_LINE];

    // CSV parsing
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

    // Initial sort by arrival time
    qsort(all_tasks, count, sizeof(Task*), compare_start_time);

    // Simulation setup
    PriorityQueue* pq = priority_queue_create(compare_tasks, hash_task);
    FILE* fout = fopen(argv[2], "w");
    if (!fout) { perror("Error opening output"); return 1; }

    int current_time = 0;
    int task_idx = 0;
    int completed = 0;

    printf("Starting simulation with %d tasks...\n", count);

    // Scheduling loop
    while (completed < count) {
        // Load arrived tasks into priority queue
        while (task_idx < count && all_tasks[task_idx]->start <= current_time) {
            priority_queue_push(pq, all_tasks[task_idx]);
            task_idx++;
        }

        if (priority_queue_size(pq) > 0) {
            // Execute highest priority task
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
            // Advance time to next task arrival
            if (task_idx < count) {
                current_time = all_tasks[task_idx]->start;
            } else {
                break;
            }
        }
    }

    printf("Simulation finished. Output saved to %s\n", argv[2]);

    // Cleanup
    fclose(fout);
    priority_queue_free(pq);
    for(int i=0; i<count; i++) free(all_tasks[i]);
    free(all_tasks);

    return 0;
}