/**
 * @file graph.h
 * @brief Generic Graph interface (Sparse implementation).
 */

#ifndef GRAPH_H
#define GRAPH_H

/** @brief Opaque structure representing the Graph. */
typedef struct graph *Graph;

/** @brief Structure representing a graph edge. */
typedef struct edge {
    void* source;   /**< Source node */
    void* dest;     /**< Destination node */
    void* label;    /**< Edge label (payload) */
} Edge;

/* --- Creation --- */

/**
 * @brief Creates a new empty graph.
 * @param labelled 1 if the graph is labelled, 0 otherwise.
 * @param directed 1 if the graph is directed, 0 otherwise.
 * @param compare Function to compare nodes.
 * @param hash Function to hash nodes.
 * @return Pointer to the new graph.
 */
Graph graph_create(int labelled, int directed, int (*compare)(const void*, const void*),
                   unsigned long (*hash)(const void*));

/**
 * @brief Checks if the graph is directed.
 * @return 1 if directed, 0 otherwise.
 */
int graph_is_directed(const Graph gr);

/**
 * @brief Checks if the graph is labelled.
 * @return 1 if labelled, 0 otherwise.
 */
int graph_is_labelled(const Graph gr);

/* --- Nodes --- */

/**
 * @brief Adds a node to the graph.
 * @param gr The graph.
 * @param node The node to add.
 * @return 1 on success, 0 if node already exists or error.
 */
int graph_add_node(Graph gr, const void* node);

/**
 * @brief Checks if a node exists.
 * @param gr The graph.
 * @param node The node to check.
 * @return 1 if present, 0 otherwise.
 */
int graph_contains_node(const Graph gr, const void* node);

/**
 * @brief Removes a node and all incident edges.
 * @param gr The graph.
 * @param node The node to remove.
 * @return 1 on success, 0 if not found.
 */
int graph_remove_node(Graph gr, const void* node);

/**
 * @brief Returns the total number of nodes.
 * @return Number of nodes.
 */
int graph_num_nodes(const Graph gr);

/**
 * @brief Retrieves all nodes in the graph.
 * @return Array of node pointers.
 */
void** graph_get_nodes(const Graph gr);

/* --- Edges --- */

/**
 * @brief Adds an edge between two nodes.
 * @param gr The graph.
 * @param node1 Source node.
 * @param node2 Destination node.
 * @param label Edge label (can be NULL if unlabelled).
 * @return 1 on success, 0 on failure.
 */
int graph_add_edge(Graph gr, const void* node1, const void* node2, const void* label);

/**
 * @brief Checks if an edge exists.
 * @param gr The graph.
 * @param node1 Source node.
 * @param node2 Destination node.
 * @return 1 if the edge exists, 0 otherwise.
 */
int graph_contains_edge(const Graph gr, const void* node1, const void* node2);

/**
 * @brief Removes an edge between two nodes.
 * @param gr The graph.
 * @param node1 Source node.
 * @param node2 Destination node.
 * @return 1 on success, 0 if edge not found.
 */
int graph_remove_edge(Graph gr, const void* node1, const void* node2);

/**
 * @brief Returns the total number of edges.
 * @return Number of edges.
 */
int graph_num_edges(const Graph gr);

/**
 * @brief Retrieves all edges in the graph.
 * @return Array of pointers to Edge structures.
 */
Edge** graph_get_edges(const Graph gr);

/**
 * @brief Retrieves the label associated with an edge.
 * @param gr The graph.
 * @param node1 Source node.
 * @param node2 Destination node.
 * @return Pointer to the label, or NULL if not found.
 */
void* graph_get_label(const Graph gr, const void* node1, const void* node2);

/* --- Neighbours --- */

/**
 * @brief Retrieves the neighbours of a specific node.
 * @param gr The graph.
 * @param node The node to query.
 * @return Array of pointers to neighbouring nodes.
 */
void** graph_get_neighbours(const Graph gr, const void* node);

/**
 * @brief Returns the number of neighbours (degree) of a node.
 * @param gr The graph.
 * @param node The node to query.
 * @return Number of neighbours.
 */
int graph_num_neighbours(const Graph gr, const void* node);

/* --- Destruction --- */

/**
 * @brief Frees the graph and all internal structures.
 * @param gr The graph to free.
 */
void graph_free(Graph gr);

#endif