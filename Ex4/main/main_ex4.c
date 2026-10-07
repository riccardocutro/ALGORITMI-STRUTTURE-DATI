#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "hash_table.h"
#include "bfs.h"

/** @brief Max line length for CSV reading. */
#define MAX_LINE 1024

/**
 * @brief Compares two strings.
 * @param a First string (void*).
 * @param b Second string (void*).
 * @return <0, 0, >0 based on lexicographical order.
 */
int compare_str(const void* a, const void* b) {
    return strcmp((const char*)a, (const char*)b);
}

/**
 * @brief Computes string hash (djb2 algorithm).
 * @param key String key (void*).
 * @return Computed hash value.
 */
unsigned long hash_str(const void* key) {
    unsigned long hash = 5381;
    int c;
    char* str = (char*)key;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

/**
 * @brief Program entry point.
 * Loads graph from CSV, runs BFS, and saves results to file.
 * @param argc Number of arguments.
 * @param argv Arguments.
 */
int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage: %s <csv_file> <start_node> <output_file>\n", argv[0]);
        return 1;
    }

    char* csv_path = argv[1];
    char* start_node_name = argv[2];
    char* output_path = argv[3];

    Graph g = graph_create(1, 0, compare_str, hash_str);

    HashTable* str_memory = hash_table_create(compare_str, hash_str);

    FILE* fin = fopen(csv_path, "r");
    if (!fin) { perror("Error opening CSV"); return 1; }

    char line[MAX_LINE];
    printf("Loading graph from %s...\n", csv_path);

    while (fgets(line, MAX_LINE, fin)) {
        char n1_buf[256], n2_buf[256];
        float dist;

        if (sscanf(line, "%[^,],%[^,],%f", n1_buf, n2_buf, &dist) == 3) {

            char* p1 = (char*)hash_table_get(str_memory, n1_buf);
            if (!p1) {
                p1 = malloc(strlen(n1_buf) + 1);
                strcpy(p1, n1_buf);
                hash_table_put(str_memory, p1, p1);
                graph_add_node(g, p1);
            }

            char* p2 = (char*)hash_table_get(str_memory, n2_buf);
            if (!p2) {
                p2 = malloc(strlen(n2_buf) + 1);
                strcpy(p2, n2_buf);
                hash_table_put(str_memory, p2, p2);
                graph_add_node(g, p2);
            }

            float* label = malloc(sizeof(float));
            *label = dist;

            graph_add_edge(g, p1, p2, label);
        }
    }
    fclose(fin);

    printf("Graph loaded: %d nodes, %d edges (undirected view)\n", graph_num_nodes(g), graph_num_edges(g));

    printf("Starting BFS from: %s\n", start_node_name);

    char* start_ptr = (char*)hash_table_get(str_memory, start_node_name);

    if (!start_ptr) {
        printf("Error: Start node '%s' not found in graph.\n", start_node_name);
        return 1;
    }

    void** visited = breadth_first_visit(g, start_ptr, compare_str, hash_str);

    // Write results
    FILE* fout = fopen(output_path, "w");
    if (!fout) { perror("Error creating output file"); return 1; }

    if (visited) {
        int i = 0;
        while(visited[i] != NULL) {
             fprintf(fout, "%s\n", (char*)visited[i]);
             i++;
        }
        printf("Visit complete. %d nodes saved to %s.\n", i, output_path);
        free(visited);
    } else {
        printf("No nodes visited.\n");
    }

    fclose(fout);

    graph_free(g);

    void** keys = hash_table_keyset(str_memory);
    size_t n = hash_table_size(str_memory);
    for(size_t i=0; i<n; i++) {
        free(keys[i]);
    }
    free(keys);
    hash_table_free(str_memory);
    return 0;
}