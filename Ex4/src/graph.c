#include "graph.h"
#include "hash_table.h"
#include <stdlib.h>
#include <stdio.h>

static int EMPTY = 0;

typedef struct {
    void* data;
    HashTable* edges;
} NodeInfo;

struct GraphType {
    HashTable* nodes;
    int directed;
    int labelled;
    int (*compare)(const void*, const void*);
    unsigned long (*hash)(const void*);
    size_t num_edges;
};

// --- Helper ---
static NodeInfo* create_node_info(Graph gr, void* data) {
    NodeInfo* info = malloc(sizeof(NodeInfo));
    info->data = data;
    info->edges = hash_table_create(gr->compare, gr->hash);
    return info;
}

static void free_node_info(NodeInfo* info) {
    if (info) {
        hash_table_free(info->edges);
        free(info);
    }
}

// --- Implementazione ---
Graph graph_create(int labelled, int directed, int (*compare)(const void*, const void*), unsigned long (*hash)(const void*)) {
    Graph gr = malloc(sizeof(struct GraphType));
    if (!gr) return NULL;

    gr->nodes = hash_table_create(compare, hash);
    gr->directed = directed;
    gr->labelled = labelled;
    gr->compare = compare;
    gr->hash = hash;
    gr->num_edges = 0;

    return gr;
}

int graph_is_directed(const Graph gr) { return gr->directed; }
int graph_is_labelled(const Graph gr) { return gr->labelled; }

int graph_add_node(Graph gr, const void* node) {
    if (!gr || !node) return 0;
    if (hash_table_contains_key(gr->nodes, node)) return 0;

    NodeInfo* info = create_node_info(gr, (void*)node);
    hash_table_put(gr->nodes, (void*)node, info);
    return 1;
}

int graph_contains_node(const Graph gr, const void* node) {
    if (!gr || !node) return 0;
    return hash_table_contains_key(gr->nodes, node);
}

int graph_num_nodes(const Graph gr) {
    return (int)hash_table_size(gr->nodes);
}

void** graph_get_nodes(const Graph gr) {
    return hash_table_keyset(gr->nodes);
}

int graph_add_edge(Graph gr, const void* node1, const void* node2, const void* label) {
    if (!gr || !node1 || !node2) return 0;
    if (gr->labelled && label == NULL) return 0;

    if (!graph_contains_node(gr, node1)) graph_add_node(gr, node1);
    if (!graph_contains_node(gr, node2)) graph_add_node(gr, node2);

    NodeInfo* src_info = (NodeInfo*)hash_table_get(gr->nodes, node1);
    if (!src_info) return 0;

    void* val_to_store = (label == NULL) ? &EMPTY : (void*)label;

    if (!hash_table_contains_key(src_info->edges, node2)) {
        hash_table_put(src_info->edges, (void*)node2, val_to_store);
        gr->num_edges++;

        if (!gr->directed) {
            NodeInfo* dest_info = (NodeInfo*)hash_table_get(gr->nodes, node2);
            if (dest_info && gr->compare(node1, node2) != 0) {
                 hash_table_put(dest_info->edges, (void*)node1, val_to_store);
            }
        }
        return 1;
    }
    return 0;
}

int graph_contains_edge(const Graph gr, const void* node1, const void* node2) {
    if (!gr || !node1 || !node2) return 0;
    NodeInfo* info = (NodeInfo*)hash_table_get(gr->nodes, node1);
    if (!info) return 0;
    return hash_table_contains_key(info->edges, node2);
}

void* graph_get_label(const Graph gr, const void* node1, const void* node2) {
    if (!graph_contains_edge(gr, node1, node2)) return NULL;
    NodeInfo* info = (NodeInfo*)hash_table_get(gr->nodes, node1);

    void* val = hash_table_get(info->edges, node2);
    if (val == &EMPTY) return NULL;
    return val;
}

int graph_remove_edge(Graph gr, const void* node1, const void* node2) {
    if (!graph_contains_edge(gr, node1, node2)) return 0;

    NodeInfo* info = (NodeInfo*)hash_table_get(gr->nodes, node1);
    hash_table_remove(info->edges, node2);
    gr->num_edges--;

    if (!gr->directed) {
        NodeInfo* info2 = (NodeInfo*)hash_table_get(gr->nodes, node2);
        if (info2 && gr->compare(node1, node2) != 0) {
            hash_table_remove(info2->edges, node1);
        }
    }
    return 1;
}

int graph_remove_node(Graph gr, const void* node) {
    if (!graph_contains_node(gr, node)) return 0;

    void** all_nodes = graph_get_nodes(gr);
    size_t count = graph_num_nodes(gr);

    for (size_t i = 0; i < count; i++) {
        void* curr = all_nodes[i];
        if (graph_contains_edge(gr, curr, node)) {
            NodeInfo* info = (NodeInfo*)hash_table_get(gr->nodes, curr);
            if (hash_table_contains_key(info->edges, node)) {
                hash_table_remove(info->edges, node);
            }
        }
    }

    NodeInfo* target_info = (NodeInfo*)hash_table_get(gr->nodes, node);
    if (gr->directed) {
        size_t out_degree = hash_table_size(target_info->edges);
        if (gr->num_edges >= out_degree) gr->num_edges -= out_degree;
        else gr->num_edges = 0;
    }

    free(all_nodes);
    free_node_info(target_info);
    hash_table_remove(gr->nodes, node);
    return 1;
}

int graph_num_edges(const Graph gr) {
    if (!gr->directed) return (int)(gr->num_edges / 2);
    return (int)gr->num_edges;
}

void** graph_get_neighbours(const Graph gr, const void* node) {
    if (!graph_contains_node(gr, node)) return NULL;
    NodeInfo* info = (NodeInfo*)hash_table_get(gr->nodes, node);
    return hash_table_keyset(info->edges);
}

int graph_num_neighbours(const Graph gr, const void* node) {
    if (!graph_contains_node(gr, node)) return 0;
    NodeInfo* info = (NodeInfo*)hash_table_get(gr->nodes, node);
    return (int)hash_table_size(info->edges);
}

Edge** graph_get_edges(const Graph gr) {
    size_t total_edges = gr->num_edges;
    if (total_edges == 0) return NULL;

    Edge** result = malloc(total_edges * sizeof(Edge*));
    void** nodes = graph_get_nodes(gr);
    size_t num_nodes = graph_num_nodes(gr);
    size_t k = 0;

    for (size_t i = 0; i < num_nodes; i++) {
        NodeInfo* info = (NodeInfo*)hash_table_get(gr->nodes, nodes[i]);
        void** neighbors = hash_table_keyset(info->edges);
        size_t num_neigh = hash_table_size(info->edges);

        for (size_t j = 0; j < num_neigh; j++) {
            Edge* e = malloc(sizeof(Edge));
            e->source = nodes[i];
            e->dest = neighbors[j];

            // Convertiamo il valore interno
            void* val = hash_table_get(info->edges, neighbors[j]);
            e->label = (val == &EMPTY) ? NULL : val;

            result[k++] = e;
        }
        free(neighbors);
    }
    free(nodes);
    return result;
}

void graph_free(Graph gr) {
    if (!gr) return;
    void** nodes = graph_get_nodes(gr);
    size_t n = graph_num_nodes(gr);
    for(size_t i=0; i<n; i++) {
        NodeInfo* info = (NodeInfo*)hash_table_get(gr->nodes, nodes[i]);
        free_node_info(info);
    }
    free(nodes);
    hash_table_free(gr->nodes);
    free(gr);
}

// --- BFS ---

typedef struct QueueNode {
    void* data;
    struct QueueNode* next;
} QueueNode;

typedef struct {
    QueueNode *head, *tail;
} Queue;

static void enqueue(Queue* q, void* data) {
    QueueNode* n = malloc(sizeof(QueueNode));
    n->data = data;
    n->next = NULL;
    if (q->tail) q->tail->next = n;
    else q->head = n;
    q->tail = n;
}

static void* dequeue(Queue* q) {
    if (!q->head) return NULL;
    QueueNode* tmp = q->head;
    void* data = tmp->data;
    q->head = tmp->next;
    if (!q->head) q->tail = NULL;
    free(tmp);
    return data;
}

static int is_empty(Queue* q) { return q->head == NULL; }

void** breadth_first_visit(Graph gr, void* start,
                           int (*compare)(const void*, const void*),
                           unsigned long (*hash)(const void*)) {

    if (!graph_contains_node(gr, start)) return NULL;

    HashTable* visited = hash_table_create(compare, hash);
    Queue q = {NULL, NULL};

    int capacity = graph_num_nodes(gr);
    void** result = malloc((capacity + 1) * sizeof(void*)); // +1 per NULL finale
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

    result[count] = NULL; // Terminatore
    hash_table_free(visited);
    return result;
}