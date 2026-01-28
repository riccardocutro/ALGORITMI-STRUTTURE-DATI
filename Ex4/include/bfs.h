#ifndef BFS_H
#define BFS_H

#include "graph.h"

void** breadth_first_visit(Graph gr, void* start, int (*compare)(const void*, const void*),
                           unsigned long (*hash)(const void*));

#endif