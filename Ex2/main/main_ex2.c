/**
 * @file main_ex2.c
 * @brief Word frequency counter using a generic Hash Table.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hash_table.h"

#define MAX_WORD_LEN 256

/**
 * @brief DJB2 hash implementation for strings.
 * @param key Pointer to the string.
 * @return Computed hash value.
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
 * @brief String comparison wrapper for generic table use.
 */
int compare_string(const void* a, const void* b) {
    return strcmp((const char*)a, (const char*)b);
}

/**
 * @brief Duplicates a string on the heap.
 * @return Pointer to new string or NULL.
 */
char* string_dup(const char* src) {
    if (src == NULL) return NULL;
    size_t len = strlen(src) + 1;
    char* dst = malloc(len);
    if (dst != NULL) memcpy(dst, src, len);
    return dst;
}

/**
 * @brief Extracts the next alphanumeric word from a file.
 * @return 1 on success, 0 on EOF.
 */
int read_next_word(FILE* f, char* buffer, size_t max_len) {
    int c;
    size_t len = 0;

    while ((c = fgetc(f)) != EOF) {
        if (isalnum(c)) {
            buffer[len++] = tolower(c);
            break;
        }
    }
    if (c == EOF) return 0;
    while ((c = fgetc(f)) != EOF) {
        if (isalnum(c)) {
            if (len < max_len - 1) {
                buffer[len++] = tolower(c);
            }
        } else {
            break;
        }
    }
    buffer[len] = '\0';
    return 1;
}

/**
 * @brief Main execution: populates table, finds max frequency, and cleans up.
 */
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

    hash_table_print_stats(table);

    // Cleanup
    hash_table_free(table);

    for (size_t i = 0; i < size; i++) {
        free(keys[i]);
        free(values[i]);
    }
    free(keys);
    free(values);
    return EXIT_SUCCESS;
}