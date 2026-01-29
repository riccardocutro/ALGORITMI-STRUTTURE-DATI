/**
 * @file test_ex1.c
 * @brief Unit tests for the hybrid sorting algorithm using the Unity framework.
 */

#include "unity.h"
#include "utils.h"
#include "sort.h"
#include <stdlib.h>
#include <string.h>

/* Setup and Teardown */

/**
 * @brief Unity framework setup routine.
 */
void setUp(void) {}

/**
 * @brief Unity framework teardown routine.
 */
void tearDown(void) {}

/**
 * @brief Helper to allocate and initialize a Record array for testing.
 * @param n Number of records to create.
 * @return Pointer to the allocated array.
 */
static Record* create_test_array(size_t n) {
    Record *arr = malloc(n * sizeof(Record));
    for (size_t i = 0; i < n; i++) {
        arr[i].id = i;
        arr[i].field1 = (float)i;
        arr[i].field2 = (int64_t)i;
        sprintf(arr[i].field3, "str_%zu", i);
    }
    return arr;
}

/**
 * @brief Validates if an array is sorted based on a comparator.
 * @return 1 if sorted, 0 otherwise.
 */
static int check_sorted(Record *arr, size_t n, int (*cmp)(const void*, const void*)) {
    for (size_t i = 0; i < n - 1; i++) {
        if (cmp(&arr[i], &arr[i+1]) > 0) return 0;
    }
    return 1;
}

/**
 * @brief Tests edge cases like NULL pointers and single-element arrays.
 */
void test_boundary_conditions(void) {
    Record r;
    hybridsort(NULL, 0, sizeof(Record), 0, compare_id);

    hybridsort(&r, 1, sizeof(Record), 0, compare_id);
    TEST_ASSERT_TRUE(1);
}

/**
 * @brief Verifies correctness when sorting a reverse-ordered array.
 */
void test_reverse_sorted(void) {
    size_t n = 1000;
    Record *arr = create_test_array(n);

    // Manually reverse
    for(size_t i=0; i<n; i++) arr[i].id = n - 1 - i;

    hybridsort(arr, n, sizeof(Record), 10, compare_id);
    TEST_ASSERT_TRUE_MESSAGE(check_sorted(arr, n, compare_id), "Reverse sorted array not sorted correctly");

    free(arr);
}

/**
 * @brief Ensures the algorithm handles arrays with duplicate keys correctly.
 */
void test_duplicates(void) {
    size_t n = 1000;
    Record *arr = malloc(n * sizeof(Record));
    for(size_t i=0; i<n; i++) {
        arr[i].id = i % 10;
    }

    hybridsort(arr, n, sizeof(Record), 10, compare_id);
    TEST_ASSERT_TRUE_MESSAGE(check_sorted(arr, n, compare_id), "Array with duplicates not sorted correctly");

    free(arr);
}

/**
 * @brief Validates the 'k' threshold logic for switching between algorithms.
 */
void test_threshold_logic(void) {
    size_t n = 50;
    Record *arr = create_test_array(n);

    for(size_t i=0; i<n; i++) arr[i].id = rand();

    hybridsort(arr, n, sizeof(Record), 100, compare_id);
    TEST_ASSERT_TRUE(check_sorted(arr, n, compare_id));

    for(size_t i=0; i<n; i++) arr[i].id = rand();

    hybridsort(arr, n, sizeof(Record), 0, compare_id);
    TEST_ASSERT_TRUE(check_sorted(arr, n, compare_id));

    free(arr);
}

/**
 * @brief Tests sorting functionality specifically for string fields.
 */
void test_string_field(void) {
    Record arr[3];
    strcpy(arr[0].field3, "Zebra");
    strcpy(arr[1].field3, "Alfa");
    strcpy(arr[2].field3, "Base");

    hybridsort(arr, 3, sizeof(Record), 0, compare_field3);

    TEST_ASSERT_EQUAL_STRING("Alfa", arr[0].field3);
    TEST_ASSERT_EQUAL_STRING("Base", arr[1].field3);
    TEST_ASSERT_EQUAL_STRING("Zebra", arr[2].field3);
}

/**
 * @brief Test runner execution point.
 */
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_boundary_conditions);
    RUN_TEST(test_reverse_sorted);
    RUN_TEST(test_duplicates);
    RUN_TEST(test_threshold_logic);
    RUN_TEST(test_string_field);
    return UNITY_END();
}