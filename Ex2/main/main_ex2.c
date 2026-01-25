#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/hash_table.h"

/**
 * @file main_ex2.c
 * @brief Application to count word frequencies using a generic Hash Table.
 */

#define MAX_WORD_LEN 256

/**
 * @brief DJB2 hash algorithm for strings.
 * @param key Pointer to the string to hash.
 * @return Unsigned long hash value.
 */
unsigned long hash_string(const void* key) {
    unsigned long hash = 5381;
    const char* str = (const char*)key;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

/**
 * @brief String comparison wrapper for strcmp.
 * @param a First string.
 * @param b Second string.
 * @return 0 if equal.
 */
int compare_string(const void* a, const void* b) {
    return strcmp((const char*)a, (const char*)b);
}

/**
 * @brief Duplicates a string in heap memory.
 * @param src Source string.
 * @return Pointer to the new string.
 */
char* string_dup(const char* src) {
    if (src == NULL) return NULL;
    size_t len = strlen(src) + 1;
    char* dst = malloc(len);
    if (dst != NULL) memcpy(dst, src, len);
    return dst;
}

/**
 * @brief Reads and cleans the next word from a file.
 * @param f File pointer.
 * @param buffer Output buffer.
 * @param max_len Buffer size.
 * @return 1 on success, 0 on EOF.
 */
int read_next_word(FILE* f, char* buffer, size_t max_len) {
    char temp[MAX_WORD_LEN];
    if (fscanf(f, "%255s", temp) != 1) return 0;

    size_t j = 0;
    for (size_t i = 0; temp[i] != '\0'; i++) {
        if (isalnum((unsigned char)temp[i])) {
            if (j < max_len - 1) buffer[j++] = tolower((unsigned char)temp[i]);
        }
    }
    buffer[j] = '\0';
    return 1;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file_path> <min_length>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* filepath = argv[1];
    int min_len = atoi(argv[2]);

    FILE* file = fopen(filepath, "r");
    if (!file) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    HashTable* table = hash_table_create(compare_string, hash_string);
    char buffer[MAX_WORD_LEN];
    size_t words_processed = 0;

    /* Populate Hash Table */
    while (read_next_word(file, buffer, MAX_WORD_LEN)) {
        if ((int)strlen(buffer) < min_len) continue;
        words_processed++;

        int* frequency = (int*)hash_table_get(table, buffer);
        if (frequency) {
            (*frequency)++;
        } else {
            int* new_freq = malloc(sizeof(int));
            char* key_copy = string_dup(buffer);
            if (new_freq && key_copy) {
                *new_freq = 1;
                hash_table_put(table, key_copy, new_freq);
            } else {
                free(new_freq); free(key_copy);
            }
        }
    }
    fclose(file);
    printf("Processed %zu valid words.\n", words_processed);

    /* Collect Keys and Values */
    void** keys = hash_table_keyset(table);
    size_t size = hash_table_size(table);
    void** values = malloc(size * sizeof(void*));

    char* max_word = NULL;
    int max_freq = 0;

    for (size_t i = 0; i < size; i++) {
        values[i] = hash_table_get(table, keys[i]);
        if (*(int*)values[i] > max_freq) {
            max_freq = *(int*)values[i];
            max_word = (char*)keys[i];
        }
    }

    if (max_word) {
        printf("RESULT: Most frequent word: '%s' (%d times)\n", max_word, max_freq);
    }

    /* Print statistics before cleaning up memory */
    hash_table_print_stats(table);

    /* --- BULLETPROOF CLEANUP --- */
    /* 1. Destroy the table structure FIRST to avoid using dangling pointers later */
    hash_table_free(table);

    /* 2. Now safely free the actual data using our cached arrays */
    for (size_t i = 0; i < size; i++) {
        free(keys[i]);   /* Free the duplicated string */
        free(values[i]); /* Free the allocated integer */
    }

    /* 3. Free the helper arrays */
    free(keys);
    free(values);

    return EXIT_SUCCESS;
}