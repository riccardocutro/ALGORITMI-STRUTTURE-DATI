#pragma once
#include <stdio.h>
#include <stdint.h>

/**
 * Data structure representing a record in the dataset.
 * Total size expected on disk: 36 bytes.
 */
typedef struct {
    uint64_t id;
    float field1;
    int64_t field2;
    char field3[16];
} __attribute__((packed)) Record;

/**
 * Reads a single record from a binary file.
 * @param file Pointer to the input file.
 * @param record Pointer to the structure to fill.
 * @return 1 on success, 0 on failure/EOF.
 */
int read_record(FILE *file, Record *record);

/**
 * Writes a single record to a binary file.
 * @param file Pointer to the output file.
 * @param record Pointer to the structure to write.
 * @return 1 on success, 0 on failure.
 */
int write_record(FILE *file, const Record *record);

/**
 * Loads all records from a file into a dynamic array
 * @param file Pointer to the open input file.
 * @param records Pointer to the array pointer.
 * @param count Pointer to a variable to store the number of records.
 * @return 0 on success, -1 on allocation fail.
 */
int load_records_from_file(FILE *file, Record **records, size_t *count);

/**
 * Saves an array of records to a file
 * @param file Pointer to the open output file
 * @param records Array of records
 * @param count Number of records to write
 * @return 0 on success, -1 on failure
 */
int save_records_to_file(FILE *file, const Record *records, size_t count);

/**
 * Prints a record to stdout (for debugging purposes).
 * @param record Pointer to the record.
 */
void print_record(const Record *record);

/* Comparison functions for qsort/hybridsort */
int compare_id(const void *a, const void *b);
int compare_field1(const void *a, const void *b);
int compare_field2(const void *a, const void *b);
int compare_field3(const void *a, const void *b);