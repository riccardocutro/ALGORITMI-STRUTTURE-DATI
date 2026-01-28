#include "unity.h"
#include "graph.h"
#include "bfs.h"
#include <string.h>
#include <stdlib.h>

Graph gr;

int compare_str(const void* a, const void* b) {
    return strcmp((const char*)a, (const char*)b);
}

unsigned long hash_str(const void* key) {
    unsigned long hash = 5381;
    int c;
    char* str = (char*)key;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

void setUp(void) {
    gr = graph_create(0, 1, compare_str, hash_str);
}

void tearDown(void) {
    graph_free(gr);
}

void test_add_contains_node(void) {
    char* n1 = "A";
    TEST_ASSERT_TRUE(graph_add_node(gr, n1));
    TEST_ASSERT_TRUE(graph_contains_node(gr, n1));
    TEST_ASSERT_FALSE(graph_contains_node(gr, "B"));
    TEST_ASSERT_EQUAL_INT(1, graph_num_nodes(gr));
}

void test_add_edge_directed(void) {
    char *a = "A", *b = "B";
    graph_add_node(gr, a);
    graph_add_node(gr, b);

    TEST_ASSERT_TRUE(graph_add_edge(gr, a, b, NULL));
    TEST_ASSERT_TRUE(graph_contains_edge(gr, a, b));
    TEST_ASSERT_FALSE(graph_contains_edge(gr, b, a));
    TEST_ASSERT_EQUAL_INT(1, graph_num_edges(gr));
}

void test_undirected_graph(void) {
    Graph ug = graph_create(0, 0, compare_str, hash_str);
    char *a = "X", *b = "Y";

    graph_add_edge(ug, a, b, NULL);
    TEST_ASSERT_TRUE(graph_contains_edge(ug, a, b));
    TEST_ASSERT_TRUE(graph_contains_edge(ug, b, a));

    graph_free(ug);
}

void test_neighbours(void) {
    char *root = "Root", *l = "L", *r = "R";
    graph_add_edge(gr, root, l, NULL);
    graph_add_edge(gr, root, r, NULL);

    TEST_ASSERT_EQUAL_INT(2, graph_num_neighbours(gr, root));
    void** neighs = graph_get_neighbours(gr, root);
    int found_l = (strcmp(neighs[0], l)==0 || strcmp(neighs[1], l)==0);
    int found_r = (strcmp(neighs[0], r)==0 || strcmp(neighs[1], r)==0);
    TEST_ASSERT_TRUE(found_l && found_r);
    free(neighs);
}

void test_bfs(void) {
    char *n1="1", *n2="2", *n3="3", *n4="4";
    graph_add_edge(gr, n1, n2, NULL);
    graph_add_edge(gr, n1, n3, NULL);
    graph_add_edge(gr, n2, n4, NULL);

    void** bfs = breadth_first_visit(gr, n1, compare_str, hash_str);

    TEST_ASSERT_EQUAL_STRING("1", bfs[0]);
    int second_is_2_or_3 = (strcmp(bfs[1], "2")==0 || strcmp(bfs[1], "3")==0);
    TEST_ASSERT_TRUE(second_is_2_or_3);

    free(bfs);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_add_contains_node);
    RUN_TEST(test_add_edge_directed);
    RUN_TEST(test_undirected_graph);
    RUN_TEST(test_neighbours);
    RUN_TEST(test_bfs);
    return UNITY_END();
}