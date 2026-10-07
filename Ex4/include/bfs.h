/**
* @file bfs.h
 * @brief Breadth-First Search (BFS) algorithm for generic graphs.
 */

#ifndef BFS_H
#define BFS_H

#include "graph.h"

/**
 * @brief Performs a Breadth-First Search (BFS) starting from a specific node.
 * * @param gr The graph to traverse.
 * @param start The starting node.
 * @param compare Comparison function for nodes.
 * @param hash Hash function for nodes.
 * @return A NULL-terminated array of visited nodes in order, or NULL if start node invalid.
 */
void** breadth_first_visit(Graph gr, void* start, int (*compare)(const void*, const void*),
                           unsigned long (*hash)(const void*));

#endif