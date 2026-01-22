#pragma once

#include <stddef.h>

/**
 * Comparison function type definition.
 * Should return an integer less than equal, or greater than zero if the
 * first argument is considered to be respectively less than, equal to, or
 * greater than the second.
 */
typedef int (*CompareFn)(const void*, const void*);

/**
 * Sorts an array using a hybrid algorithm (QuickSort + SelectionSort).
 * * The algorithm uses QuickSort for partitions larger than or equal to the
 * threshold k, and switches to SelectionSort for partitions smaller than k.
 * @param base   Pointer to the first element of the array to be sorted.
 * @param nitems Number of elements in the array.
 * @param size   Size in bytes of each element.
 * @param k      Threshold value. If a partition size is < k, SelectionSort is used.
 * @param compar Pointer to the comparison function.
 */
void hybridsort(void *base, size_t nitems, size_t size, size_t k, CompareFn compar);