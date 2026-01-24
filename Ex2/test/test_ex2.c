#include "unity.h"
#include "hash_table.h"
#include <string.h>
#include <stdlib.h>

/* --- HELPER FUNCTIONS FOR INTEGERS --- */

// Simple hash for integers: just returns the number itself
unsigned long hash_int(const void* key) {
    return *(const int*)key;
}

// Compare two integers
int compare_int(const void* a, const void* b) {
    int i_a = *(const int*)a;
    int i_b = *(const int*)b;
    return i_a - i_b;
}

//helpers

unsigned long hash_string(const void* key) {
    unsigned long hash = 5381;
    const char* str = (const char*)key;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

// Wrapper for strcmp
int compare_string(const void* a, const void* b) {
    return strcmp((const char*)a, (const char*)b);
}

unsigned long hash_dummy_constant(const void* key) {
    (void)key;
    return 1;
}

HashTable* table;

void setUp(void) {
    table = hash_table_create(compare_int, hash_int);
}

void tearDown(void) {
    hash_table_free(table);
}

//tests

void test_create_table(void) {
    TEST_ASSERT_NOT_NULL(table);
    TEST_ASSERT_EQUAL_INT(0, hash_table_size(table));
}

void test_put_get_integer(void) {
    int key = 42;
    int val = 100;

    hash_table_put(table, &key, &val);

    TEST_ASSERT_EQUAL_INT(1, hash_table_size(table));
    TEST_ASSERT_TRUE(hash_table_contains_key(table, &key));

    int* retrieved = (int*)hash_table_get(table, &key);
    TEST_ASSERT_NOT_NULL(retrieved);
    TEST_ASSERT_EQUAL_INT(100, *retrieved);
}

void test_update_existing_key(void) {
    int key = 10;
    int val1 = 500;
    int val2 = 999;

    hash_table_put(table, &key, &val1);
    TEST_ASSERT_EQUAL_INT(1, hash_table_size(table));

    hash_table_put(table, &key, &val2);
    TEST_ASSERT_EQUAL_INT(1, hash_table_size(table));

    int* retrieved = (int*)hash_table_get(table, &key);
    TEST_ASSERT_EQUAL_INT(999, *retrieved);
}

void test_remove(void) {
    int k1 = 1, v1 = 10;
    int k2 = 2, v2 = 20;

    hash_table_put(table, &k1, &v1);
    hash_table_put(table, &k2, &v2);
    TEST_ASSERT_EQUAL_INT(2, hash_table_size(table));

    hash_table_remove(table, &k1);
    TEST_ASSERT_EQUAL_INT(1, hash_table_size(table));
    TEST_ASSERT_FALSE(hash_table_contains_key(table, &k1));
    TEST_ASSERT_TRUE(hash_table_contains_key(table, &k2));

    hash_table_remove(table, &k1);
    TEST_ASSERT_EQUAL_INT(1, hash_table_size(table));
}

void test_string_keys(void) {
    hash_table_free(table);
    table = hash_table_create(compare_string, hash_string);

    char* k1 = "hello";
    char* v1 = "world";
    char* k2 = "foo";
    char* v2 = "bar";

    hash_table_put(table, k1, v1);
    hash_table_put(table, k2, v2);
    TEST_ASSERT_EQUAL_STRING("world", (char*)hash_table_get(table, "hello"));
    TEST_ASSERT_EQUAL_STRING("bar", (char*)hash_table_get(table, "foo"));
    TEST_ASSERT_NULL(hash_table_get(table, "missing"));
}

void test_collision_handling(void) {
    hash_table_free(table);
    table = hash_table_create(compare_int, hash_dummy_constant);

    int k1 = 1, v1 = 100;
    int k2 = 2, v2 = 200;
    int k3 = 3, v3 = 300;

    hash_table_put(table, &k1, &v1);
    hash_table_put(table, &k2, &v2);
    hash_table_put(table, &k3, &v3);

    TEST_ASSERT_EQUAL_INT(3, hash_table_size(table));
    TEST_ASSERT_EQUAL_INT(100, *(int*)hash_table_get(table, &k1));
    TEST_ASSERT_EQUAL_INT(200, *(int*)hash_table_get(table, &k2));
    TEST_ASSERT_EQUAL_INT(300, *(int*)hash_table_get(table, &k3));

    hash_table_remove(table, &k2);
    TEST_ASSERT_EQUAL_INT(2, hash_table_size(table));
    TEST_ASSERT_NULL(hash_table_get(table, &k2));
    TEST_ASSERT_EQUAL_INT(100, *(int*)hash_table_get(table, &k1));
    TEST_ASSERT_EQUAL_INT(300, *(int*)hash_table_get(table, &k3));
}

void test_keyset(void) {
    int k1 = 10, v1 = 1;
    int k2 = 20, v2 = 2;

    hash_table_put(table, &k1, &v1);
    hash_table_put(table, &k2, &v2);
    void** keys = hash_table_keyset(table);
    TEST_ASSERT_NOT_NULL(keys);
    int count = 0;
    for(int i=0; i<2; i++) {
        int val = *(int*)keys[i];
        if (val == 10 || val == 20) count++;
    }
    TEST_ASSERT_EQUAL_INT(2, count);
    free(keys);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create_table);
    RUN_TEST(test_put_get_integer);
    RUN_TEST(test_update_existing_key);
    RUN_TEST(test_remove);
    RUN_TEST(test_string_keys);
    RUN_TEST(test_collision_handling);
    RUN_TEST(test_keyset);
    return UNITY_END();
}