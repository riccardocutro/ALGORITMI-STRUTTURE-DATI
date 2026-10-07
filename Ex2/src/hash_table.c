/**
 * @file hash_table.c
 * @brief Implementation of a generic Hash Table using separate chaining.
 */

#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 127
#define LOAD_FACTOR 0.75
#define SHRINK_THRESHOLD 0.25

/**
 * @brief Internal node structure for separate chaining.
 */
typedef struct Node {
    void* key;
    void* value;
    struct Node* next;
} Node;

/**
 * @brief Hash Table structure definition.
 */
struct HashTable {
    Node** buckets;
    size_t capacity;
    size_t size;
    CompareHashFn compare;
    HashFn hash;
};

/**
 * @brief Allocates and initializes a new Hash Table.
 * @param cmp_fn Function to compare keys.
 * @param hash_fn Function to compute hash values.
 * @return Pointer to the new table or NULL on failure.
 */
HashTable* hash_table_create(CompareHashFn cmp_fn, HashFn hash_fn) {
    if (!cmp_fn || !hash_fn) return NULL;

    HashTable* table = malloc(sizeof(HashTable));
    if (!table) return NULL;

    table->capacity = INITIAL_CAPACITY;
    table->size = 0;
    table->compare = cmp_fn;
    table->hash = hash_fn;

    table->buckets = calloc(table->capacity, sizeof(Node*));
    if (!table->buckets) {
        free(table);
        return NULL;
    }

    return table;
}

/**
 * @brief Frees all nodes and the table structure.
 * Note: Does not free the keys or values themselves.
 */
void hash_table_free(HashTable* table) {
    if (!table) return;

    for (size_t i = 0; i < table->capacity; ++i) {
        Node* current = table->buckets[i];
        while (current) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(table->buckets);
    free(table);
}

/**
 * @brief Helper to compute the bucket index for a key.
 */
static size_t get_bucket_index(HashFn hash_fn, const void* key, size_t capacity) {
    unsigned long hash_val = hash_fn(key);
    return hash_val % capacity;
}

/**
 * @brief Internal function to resize the table when load factor is exceeded.
 */
static void hash_table_resize(HashTable* table, size_t new_capacity) {
    Node** new_buckets = calloc(new_capacity, sizeof(Node*));
    if (!new_buckets) return;

    for (size_t i = 0; i < table->capacity; ++i) {
        Node* current = table->buckets[i];
        while (current) {
            Node* next = current->next;
            size_t new_index = get_bucket_index(table->hash, current->key, new_capacity);
            current->next = new_buckets[new_index];
            new_buckets[new_index] = current;
            current = next;
        }
    }
    free(table->buckets);
    table->buckets = new_buckets;
    table->capacity = new_capacity;
}

/**
 * @brief Inserts or updates a key-value pair.
 * Triggers a resize if the load factor threshold is reached.
 */
void hash_table_put(HashTable* table, const void* key, const void* value) {
    if (table->size >= table->capacity * LOAD_FACTOR) {
        hash_table_resize(table, table->capacity * 2);
    }

    size_t index = get_bucket_index(table->hash, key, table->capacity);
    Node* current = table->buckets[index];

    while (current) {
        if (table->compare(current->key, key) == 0) {
            current->value = (void *) value;
            return;
        }
        current = current->next;
    }

    Node* new_node = malloc(sizeof(Node));
    if (!new_node) return;

    new_node->key = (void*)key;
    new_node->value = (void*)value;
    new_node->next = table->buckets[index];
    table->buckets[index] = new_node;
    table->size++;
}

/**
 * @brief Retrieves the value associated with a key.
 * @return Pointer to the value or NULL if not found.
 */
void* hash_table_get(const HashTable* table, const void* key) {
    size_t index = get_bucket_index(table->hash, key, table->capacity);
    Node* current = table->buckets[index];

    while (current) {
        if (table->compare(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}

/**
 * @brief Checks if a key exists in the table.
 * @return 1 if found, 0 otherwise.
 */
int hash_table_contains_key(const HashTable* table, const void* key) {
    return (hash_table_get(table, key) != NULL);
}

/**
 * @brief Removes a key and its associated value from the table.
 */
void hash_table_remove(HashTable* table, const void* key) {
    size_t index = get_bucket_index(table->hash, key, table->capacity);
    Node* current = table->buckets[index];
    Node* prev = NULL;

    while (current) {
        if (table->compare(current->key, key) == 0) {
            if (prev) {
                prev->next = current->next;
            } else {
                table->buckets[index] = current->next;
            }
            free(current);
            table->size--;
            if (table->capacity > INITIAL_CAPACITY && (double)table->size / table->capacity
                <= SHRINK_THRESHOLD) {

                size_t new_capacity = table->capacity / 2;

                if (new_capacity < INITIAL_CAPACITY) {
                    new_capacity = INITIAL_CAPACITY;
                }

                hash_table_resize(table, new_capacity);
                }
            return;
        }
        prev = current;
        current = current->next;
    }
}
/**
 * @brief Returns the number of elements in the table.
 */
size_t hash_table_size(const HashTable* table) {
    return table->size;
}

/**
 * @brief Returns an array of all keys currently in the table.
 * @return Pointer to an array of keys (caller must free).
 */
void** hash_table_keyset(const HashTable* table) {
    if (table->size == 0) return NULL;
    void** keys = malloc(table->size * sizeof(void*));
    if (!keys) return NULL;
    size_t count = 0;
    for (size_t i = 0; i < table->capacity; ++i) {
        Node* current = table->buckets[i];
        while (current) {
            keys[count++] = current->key;
            current = current->next;
        }
    }
    return keys;
}

/**
 * @brief Prints technical performance statistics to stdout.
 */
void hash_table_print_stats(const HashTable* table) {
    if (table == NULL) return;

    size_t occupied_buckets = 0;
    size_t total_collisions = 0;
    size_t max_chain_len = 0;

    for (size_t i = 0; i < table->capacity; i++) {
        if (table->buckets[i] != NULL) {
            occupied_buckets++;
            size_t current_chain_len = 0;
            Node* current = table->buckets[i];

            while (current) {
                current_chain_len++;
                current = current->next;
            }

            if (current_chain_len > 1) {
                total_collisions += (current_chain_len - 1);
            }
            if (current_chain_len > max_chain_len) {
                max_chain_len = current_chain_len;
            }
        }
    }

    printf("\n--- HASH TABLE TECHNICAL STATS ---\n");
    printf("Total capacity:      %zu\n", table->capacity);
    printf("Unique keys (size):  %zu\n", table->size);
    printf("Occupied buckets:    %zu\n", occupied_buckets);
    printf("Total collisions:    %zu\n", total_collisions);
    printf("Longest chain:       %zu\n", max_chain_len);
    printf("Load Factor:         %.2f (Target: 0.75)\n", (double)table->size / table->capacity);
    printf("----------------------------------\n");
}