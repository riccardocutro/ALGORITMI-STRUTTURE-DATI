/**
 * @file test_ex3.c
 * @brief Unit tests for the Priority Queue implementation using Unity.
 */

#include "unity.h"
#include "priority_queue.h"
#include <stdlib.h>

PriorityQueue* pq;

/**
 * @brief Comparator for integer priorities.
 * @return Difference between a and b (Max-Heap logic).
 */
int compare_int(const void* a, const void* b) {
    return (*(int*)a) - (*(int*)b);
}

/**
 * @brief Hash function for integer keys.
 */
unsigned long hash_int(const void* key) {
    return *(unsigned long*)key;
}

/**
 * @brief Test setup: creates a fresh Priority Queue before each test.
 */
void setUp(void) {
    pq = priority_queue_create(compare_int, hash_int);
}

/**
 * @brief Test teardown: destroys the Priority Queue after each test.
 */
void tearDown(void) {
    priority_queue_free(pq);
}

/**
 * @brief Verifies that a new queue is not NULL and starts empty.
 */
void test_create_empty(void) {
    TEST_ASSERT_NOT_NULL(pq);
    TEST_ASSERT_EQUAL_INT(0, priority_queue_size(pq));
}

/**
 * @brief Tests basic push, top, and pop operations to verify heap ordering.
 */
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

/**
 * @brief Validates the constant-time membership check.
 */
void test_contains(void) {
    int a = 1, b = 2;
    priority_queue_push(pq, &a);
    TEST_ASSERT_TRUE(priority_queue_contains(pq, &a));
    TEST_ASSERT_FALSE(priority_queue_contains(pq, &b));
}

/**
 * @brief Tests the removal of an arbitrary element from the middle of the heap.
 */
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

/**
 * @brief Test runner for the priority queue unit tests.
 */
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create_empty);
    RUN_TEST(test_push_top_pop);
    RUN_TEST(test_contains);
    RUN_TEST(test_remove_middle);
    return UNITY_END();
}