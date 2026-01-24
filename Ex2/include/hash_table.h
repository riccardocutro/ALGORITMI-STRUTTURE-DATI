#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>

typedef struct HashTable HashTable;

typedef int (*CompareHashFn)(const void* a, const void* b);
typedef unsigned long (*HashFn)(const void* key);

/**
 * Creates a new hash table.
 * @param cmp_fn Function to compare keys.
 * @param hash_fn Function to calculate the hash of keys.
 * @return Pointer to the new table, or NULL if there is an error.
 */
HashTable* hash_table_create(CompareHashFn cmp_fn, HashFn hash_fn);

/**
 * Inserts a key-value pair.
 * If the key already exists, the value is updated (overwritten).
 * Note: The table does not copy the data, it only saves the pointers.
 */
void hash_table_put(HashTable* table, const void* key, const void* value);

/**
 * Gets the value associated with a key.
 * @return The pointer to the value, or NULL if the key is not found.
 */
void* hash_table_get(const HashTable* table, const void* key);

/**
 * Checks if a key is in the table.
 * @return 1 if found, 0 otherwise.
 */
int hash_table_contains_key(const HashTable* table, const void* key);

/**
 * Removes the key (and its value) from the table.
 * If the key is not there, it does nothing.
 */
void hash_table_remove(HashTable* table, const void* key);



#endif