/**
 * @file utils.c
 * @brief Utility functions for record management, I/O, and comparison.
 */

#include "utils.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief Reads a single record from a file stream.
 * @return 1 on success, 0 on failure.
 */
int read_record(FILE *file, Record *record) {
    if (!file || !record) return 0;
    size_t read = fread(record, sizeof(Record), 1, file);
    return (read == 1);
}

/**
 * @brief Writes a single record to a file stream.
 * @return 1 on success, 0 on failure.
 */
int write_record(FILE *file, const Record *record) {
    if (!file || !record) return 0;
    size_t written = fwrite(record, sizeof(Record), 1, file);
    return (written == 1);
}

/**
 * @brief Loads all records from a file into a newly allocated array.
 * @return 0 on success, -1 on error.
 */
int load_records_from_file(FILE *file, Record **records, size_t *count) {
    if (!file || !records || !count) return -1;

    if (fseek(file, 0, SEEK_END) != 0) return -1;
    long filesize = ftell(file);
    rewind(file);

    if (filesize < 0 || (filesize % sizeof(Record)) != 0) return -1;

    size_t num_records = filesize / sizeof(Record);

    *records = malloc(num_records * sizeof(Record));
    if (*records == NULL) return -1;

    size_t read_count = fread(*records, sizeof(Record), num_records, file);
    if (read_count != num_records) {
        free(*records);
        *records = NULL;
        return -1;
    }

    *count = num_records;
    return 0;
}

/**
 * @brief Saves an array of records to a file.
 * @return 0 on success, -1 on error.
 */
int save_records_to_file(FILE *file, const Record *records, size_t count) {
    if (!file || !records) return -1;
    size_t written = fwrite(records, sizeof(Record), count, file);
    return (written == count) ? 0 : -1;
}

/**
 * @brief Prints record fields to standard output.
 */
void print_record(const Record *record) {
    if (!record) return;
    printf("ID: %lu | F1: %.2f | F2: %ld | F3: %s\n",
           record->id, record->field1, record->field2, record->field3);
}

/**
 * @brief Compares two records by their ID field.
 * @return Negative if a < b, positive if a > b, 0 if equal.
 */
int compare_id(const void *a, const void *b) {
    const Record *ra = (const Record *)a;
    const Record *rb = (const Record *)b;

    if (ra->id < rb->id) return -1;
    if (ra->id > rb->id) return 1;
    return 0;
}

/**
 * @brief Compares two records by field1 (float).
 */
int compare_field1(const void *a, const void *b) {
    const Record *ra = (const Record *)a;
    const Record *rb = (const Record *)b;

    if (ra->field1 < rb->field1) return -1;
    if (ra->field1 > rb->field1) return 1;
    return 0;
}

/**
 * @brief Compares two records by field2 (long).
 */
int compare_field2(const void *a, const void *b) {
    const Record *ra = (const Record *)a;
    const Record *rb = (const Record *)b;

    if (ra->field2 < rb->field2) return -1;
    if (ra->field2 > rb->field2) return 1;
    return 0;
}

/**
 * @brief Compares two records by field3 (string) using strncmp.
 */
int compare_field3(const void *a, const void *b) {
    const Record *ra = (const Record *)a;
    const Record *rb = (const Record *)b;
    return strncmp(ra->field3, rb->field3, sizeof(ra->field3));
}