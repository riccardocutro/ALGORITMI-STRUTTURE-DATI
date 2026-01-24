#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hash_table.h"

#define MAX_WORD_LEN 256

unsigned long hash_string(const void* key) {
    unsigned long hash = 5381;
    const char* str = (const char*)key;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

int compare_string(const void* a, const void* b) {
    return strcmp((const char*)a, (const char*)b);
}

/* --- HELPER FUNCTIONS --- */

/**
 * Allocates memory for a copy of the string and copies it.
 */
char* string_dup(const char* src) {
    if (src == NULL) return NULL;
    size_t len = strlen(src) + 1;
    char* dst = malloc(len);
    if (dst != NULL) {
        memcpy(dst, src, len);
    }
    return dst;
}

/**
 * Reads a word from the file and cleans it.
 * Returns 1 if a word was read, 0 on EOF.
 */
int read_next_word(FILE* f, char* buffer, size_t max_len) {
    char temp[MAX_WORD_LEN];
    if (fscanf(f, "%255s", temp) != 1) {
        return 0;
    }

    size_t j = 0;
    for (size_t i = 0; temp[i] != '\0'; i++) {
        if (isalnum((unsigned char)temp[i])) {
            if (j < max_len - 1) {
                buffer[j++] = tolower((unsigned char)temp[i]);
            }
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

    printf("Analyzing file: %s (Min length: %d)\n", filepath, min_len);

    FILE* file = fopen(filepath, "r");
    if (!file) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }
    HashTable* table = hash_table_create(compare_string, hash_string);
    if (!table) {
        fprintf(stderr, "Error creating hash table\n");
        fclose(file);
        return EXIT_FAILURE;
    }


    char buffer[MAX_WORD_LEN];
    size_t words_processed = 0;

    while (read_next_word(file, buffer, MAX_WORD_LEN)) {
        if ((int)strlen(buffer) < min_len) {
            continue;
        }

        words_processed++;

        int* frequency = (int*)hash_table_get(table, buffer);

        if (frequency) {
            (*frequency)++;
        } else {
            int* new_freq = malloc(sizeof(int));
            if (!new_freq) {
                perror("Malloc failed");
                break;
            }
            *new_freq = 1;

            char* key_copy = string_dup(buffer);
            if (!key_copy) {
                perror("String duplication failed");
                free(new_freq);
                break;
            }

            hash_table_put(table, key_copy, new_freq);
        }
    }

    fclose(file);
    printf("Processed %zu valid words.\n", words_processed);

    void** keys = hash_table_keyset(table);
    size_t size = hash_table_size(table);

    char* max_word = NULL;
    int max_freq = 0;

    for (size_t i = 0; i < size; i++) {
        char* key = (char*)keys[i];
        int* val = (int*)hash_table_get(table, key);

        if (*val > max_freq) {
            max_freq = *val;
            max_word = key;
        }
    }

    if (max_word) {
        printf("\nRESULT:\n");
        printf("Most frequent word (>= %d chars): '%s' (appears %d times)\n", min_len, max_word, max_freq);
    } else {
        printf("\nNo words found with length >= %d.\n", min_len);
    }

    //clean memory
    for (size_t i = 0; i < size; i++) {
        char* key = (char*)keys[i];
        int* val = (int*)hash_table_get(table, key);

        free(val);
        free(key);
    }
    free(keys);
    hash_table_free(table);

    return EXIT_SUCCESS;
}