#include "unity.h"
#include "priority_queue.h"
#include <stdlib.h>

PriorityQueue* pq;

int compare_int(const void* a, const void* b) {
    return (*(int*)a) - (*(int*)b);
}

unsigned long hash_int(const void* key) {
    return *(unsigned long*)key;
}

void setUp(void) {
    pq = priority_queue_create(compare_int, hash_int);
}

void tearDown(void) {
    priority_queue_free(pq);
}

void test_create_empty(void) {
    TEST_ASSERT_NOT_NULL(pq);
    TEST_ASSERT_EQUAL_INT(0, priority_queue_size(pq));
}

void test_push_top_pop(void) {
    int v1 = 10, v2 = 20, v3 = 5;

    priority_queue_push(pq, &v1);
    TEST_ASSERT_EQUAL_INT(10, *(int*)priority_queue_top(pq));
    priority_queue_push(pq, &v2);
    TEST_ASSERT_EQUAL_INT(20, *(int*)priority_queue_top(pq));
    priority_queue_push(pq, &v3);
    TEST_ASSERT_EQUAL_INT(20, *(int*)priority_queue_top(pq));
    priority_queue_pop(pq);
    TEST_ASSERT_EQUAL_INT(10, *(int*)priority_queue_top(pq));
    priority_queue_pop(pq);
    TEST_ASSERT_EQUAL_INT(5, *(int*)priority_queue_top(pq));
}

void test_contains(void) {
    int a = 1, b = 2;
    priority_queue_push(pq, &a);
    TEST_ASSERT_TRUE(priority_queue_contains(pq, &a));
    TEST_ASSERT_FALSE(priority_queue_contains(pq, &b));
}

void test_remove_middle(void) {
    int v1 = 50, v2 = 40, v3 = 30, v4 = 20;
    priority_queue_push(pq, &v1);
    priority_queue_push(pq, &v2);
    priority_queue_push(pq, &v3);
    priority_queue_push(pq, &v4);
    TEST_ASSERT_TRUE(priority_queue_remove(pq, &v2));
    TEST_ASSERT_FALSE(priority_queue_contains(pq, &v2));
    TEST_ASSERT_EQUAL_INT(3, priority_queue_size(pq));
    TEST_ASSERT_EQUAL_INT(50, *(int*)priority_queue_top(pq));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create_empty);
    RUN_TEST(test_push_top_pop);
    RUN_TEST(test_contains);
    RUN_TEST(test_remove_middle);
    return UNITY_END();
}