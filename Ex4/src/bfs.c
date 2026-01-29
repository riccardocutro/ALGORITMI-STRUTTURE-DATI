#include "bfs.h"
#include "graph.h"
#include "hash_table.h"
#include <stdlib.h>
#include <stdio.h>

/** @brief Queue node for BFS traversal. */
typedef struct QueueNode {
    void* data;
    struct QueueNode* next;
} QueueNode;

/** @brief Simple FIFO Queue structure. */
typedef struct {
    QueueNode *head, *tail;
} Queue;

/** @brief Adds an element to the tail of the queue. */
static void enqueue(Queue* q, void* data) {
    QueueNode* n = malloc(sizeof(QueueNode));
    n->data = data;
    n->next = NULL;
    if (q->tail) q->tail->next = n;
    else q->head = n;
    q->tail = n;
}

/** @brief Removes and returns the element from the head of the queue. */
static void* dequeue(Queue* q) {
    if (!q->head) return NULL;
    QueueNode* tmp = q->head;
    void* data = tmp->data;
    q->head = tmp->next;
    if (!q->head) q->tail = NULL;
    free(tmp);
    return data;
}
/** @brief Checks if the queue is empty. */
static int is_empty(Queue* q) { return q->head == NULL; }

// --- Impl BFS ---

void** breadth_first_visit(Graph gr, void* start,
                           int (*compare)(const void*, const void*),
                           unsigned long (*hash)(const void*)) {

    if (!graph_contains_node(gr, start)) return NULL;

    HashTable* visited = hash_table_create(compare, hash);
    Queue q = {NULL, NULL};

    int capacity = graph_num_nodes(gr);
    void** result = malloc((capacity + 1) * sizeof(void*));
    int count = 0;

    enqueue(&q, start);
    hash_table_put(visited, start, (void*)1);

    while (!is_empty(&q)) {
        void* curr = dequeue(&q);
        result[count++] = curr;

        int n_neigh = graph_num_neighbours(gr, curr);
        if (n_neigh > 0) {
            void** neighbors = graph_get_neighbours(gr, curr);
            for (int i = 0; i < n_neigh; i++) {
                if (!hash_table_contains_key(visited, neighbors[i])) {
                    hash_table_put(visited, neighbors[i], (void*)1);
                    enqueue(&q, neighbors[i]);
                }
            }
            free(neighbors);
        }
    }

    result[count] = NULL;

    hash_table_free(visited);
    return result;
}