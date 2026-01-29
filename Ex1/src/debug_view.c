/**
 * @file debug_view.c
 * @brief Utility to inspect the first and last records of a binary file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../include/utils.h"

/**
 * @brief Prints a single record to stdout.
 * @param r Pointer to the Record.
 */
void print_single_record(const Record *r) {
    printf("ID: %-10lu | F1: %-10.2f | F2: %-10ld | F3: %-10.10s\n",
           (unsigned long)r->id, r->field1, (long)r->field2, r->field3);
}

/**
 * @brief Main entry point: displays head and tail of a binary file.
 * @return 0 on success, 1 on error.
 */
int main(int argc, char *argv[]) {
    // Argument validation
    if (argc < 2) {
        printf("Utilizzo: %s <file_binario> [numero_record]\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    size_t n_to_show = (argc == 3) ? (size_t)atoi(argv[2]) : 20;

    // File stream initialization
    FILE *f = fopen(filename, "rb");
    if (!f) {
        perror("Errore apertura file");
        return 1;
    }

    // Determine file size and total record count
    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    size_t total_records = (size_t)(file_size / sizeof(Record));
    rewind(f);

    printf("File: %s | Record totali: %zu\n", filename, total_records);
    printf("--- PRIMI %zu RECORD ---\n", n_to_show);

    // Read and display the beginning of the file
    Record r;
    for (size_t i = 0; i < n_to_show && i < total_records; i++) {
        if (fread(&r, sizeof(Record), 1, f) == 1) {
            printf("[%zu] ", i);
            print_single_record(&r);
        }
    }

    // Seek and display the end of the file if it exceeds n_to_show
    if (total_records > n_to_show) {
        printf("\n... (omessi record intermedi) ...\n\n");
        printf("--- ULTIMI %zu RECORD ---\n", n_to_show);

        fseek(f, -((long)sizeof(Record) * (long)n_to_show), SEEK_END);
        for (size_t i = 0; i < n_to_show; i++) {
            if (fread(&r, sizeof(Record), 1, f) == 1) {
                printf("[%zu] ", total_records - n_to_show + i);
                print_single_record(&r);
            }
        }
    }

    fclose(f);
    return 0;
}