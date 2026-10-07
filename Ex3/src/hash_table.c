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

/**
 * @brief Node structure for the linked list in each bucket.
 */
typedef struct Node {
    void* key;      /**< Pointer to the key */
    void* value;    /**< Pointer to the value */
    struct Node* next; /**< Pointer to the next node in the chain */
} Node;

/**
 * @brief Hash Table structure definition.
 */
struct HashTable {
    Node** buckets;         /**< Array of pointers to nodes */
    size_t capacity;        /**< Total number of buckets */
    size_t size;            /**< Number of elements in the table */
    CompareHashFn compare;  /**< Function to compare keys */
    HashFn hash;            /**< Function to compute hash values */
};

/**
 * @brief Allocates memory for a new table and its buckets.
 * @param cmp_fn Key comparison function.
 * @param hash_fn Hashing function.
 * @return Pointer to the new table, or NULL on failure.
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
 * @brief Deletes all nodes and the table to prevent memory leaks.
 * @note Does not free the memory of keys or values.
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
 * @brief Internal helper to calculate the bucket index for a key.
 */
static size_t get_bucket_index(HashFn hash_fn, const void* key, size_t capacity) {
    unsigned long hash_val = hash_fn(key);
    return hash_val % capacity;
}

/**
 * @brief Resizes the hash table to a new capacity and rehashes existing keys.
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
 * @brief Adds a new key-value pair or updates an existing one.
 */
void hash_table_put(HashTable* table, const void* key, const void* value) {
    if (table->size >= table->capacity * LOAD_FACTOR) {
        hash_table_resize(table, table->capacity * 2);
    }

    size_t index = get_bucket_index(table->hash, key, table->capacity);
    Node* current = table->buckets[index];

    while (current) {
        if (table->compare(current->key, key) == 0) {
            current->value = (void*)value;
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
 * @brief Finds and returns the value associated with a key.
 * @return Pointer to the value, or NULL if not found.
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
 * @brief Checks if the table contains a specific key.
 * @return 1 if found, 0 otherwise.
 */
int hash_table_contains_key(const HashTable* table, const void* key) {
    return (hash_table_get(table, key) != NULL);
}

/**
 * @brief Removes a key and its associated node from the table.
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
 * @brief Returns an array containing all keys in the table.
 * @note Caller is responsible for freeing the returned array.
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
 * @brief Traverses the table to calculate occupancy and collision metrics.
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