#include "unity.h"
#include "utils.h"
#include "sort.h"
#include <stdlib.h>
#include <string.h>

/* Setup and Teardown */
void setUp(void) {}
void tearDown(void) {}

/* Helper function to create test data */
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

static int check_sorted(Record *arr, size_t n, int (*cmp)(const void*, const void*)) {
    for (size_t i = 0; i < n - 1; i++) {
        if (cmp(&arr[i], &arr[i+1]) > 0) return 0;
    }
    return 1;
}

/* Test 1: Boundary conditions */
void test_boundary_conditions(void) {
    Record r;
    hybridsort(NULL, 0, sizeof(Record), 0, compare_id);

    hybridsort(&r, 1, sizeof(Record), 0, compare_id);
    TEST_ASSERT_TRUE(1);
}

/* Test 2: Reverse sorted array  */
void test_reverse_sorted(void) {
    size_t n = 1000;
    Record *arr = create_test_array(n);

    // Manually reverse
    for(size_t i=0; i<n; i++) arr[i].id = n - 1 - i;

    hybridsort(arr, n, sizeof(Record), 10, compare_id);
    TEST_ASSERT_TRUE_MESSAGE(check_sorted(arr, n, compare_id), "Reverse sorted array not sorted correctly");

    free(arr);
}

/* Test 3: Array with many duplicates  */
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

/* Test 4: Threshold K logic (Selection Sort vs Quick Sort) */
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

/* Test 5: Sorting on different fields (Strings) */
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

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_boundary_conditions);
    RUN_TEST(test_reverse_sorted);
    RUN_TEST(test_duplicates);
    RUN_TEST(test_threshold_logic);
    RUN_TEST(test_string_field);
    return UNITY_END();
}