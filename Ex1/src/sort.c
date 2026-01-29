/**
 * @file sort.c
 * @brief Implementation of a hybrid sorting algorithm (QuickSort + Selection Sort).
 */

#include "sort.h"
#include <stdlib.h>
#include <string.h>


/**
 * @brief Swaps two elements of a given size.
 * Uses a static buffer for small elements to improve performance.
 * @param a Pointer to the first element.
 * @param b Pointer to the second element.
 * @param size Size of each element in bytes.
 */
static void swap(void *a, void *b, size_t size) {
    if (a == b) return;

    char buffer[256];
    if (size <= sizeof(buffer)) {
        memcpy(buffer, a, size);
        memcpy(a, b, size);
        memcpy(b, buffer, size);
    } else {
        void *temp = malloc(size);
        if (temp) {
            memcpy(temp, a, size);
            memcpy(a, b, size);
            memcpy(b, temp, size);
            free(temp);
        }
    }
}

/**
 * @brief Standard O(n^2) Selection Sort algorithm.
 * @param base Pointer to the first element.
 * @param nitems Number of elements.
 * @param size Size of each element.
 * @param compar Comparison function.
 */
static void selection_sort(void *base, size_t nitems, size_t size, CompareFn compar) {
    char *base_ptr = (char *)base;
    for (size_t i = 0; i < nitems - 1; ++i) {
        size_t min_idx = i;
        char *p_min = base_ptr + (min_idx * size);

        for (size_t j = i + 1; j < nitems; ++j) {
            char *p_curr = base_ptr + (j * size);
            if (compar(p_curr, p_min) < 0) {
                min_idx = j;
                p_min = p_curr;
            }
        }
        if (min_idx != i) {
            swap(base_ptr + (i * size), p_min, size);
        }
    }
}

/**
 * @brief Selects a pivot using the Median-of-Three strategy.
 * Sorts the first, middle, and last elements and moves the median to the end.
 * @param base Pointer to the first element.
 * @param nitems Number of elements.
 * @param size Size of each element.
 * @param compar Comparison function.
 */
static void median_of_three(void *base, size_t nitems, size_t size, CompareFn compar) {
    char *base_ptr = (char *)base;
    size_t mid = (nitems / 2);

    char *p_first = base_ptr;
    char *p_mid   = base_ptr + (mid * size);
    char *p_last  = base_ptr + ((nitems - 1) * size);

    if (compar(p_mid, p_first) < 0) swap(p_mid, p_first, size);
    if (compar(p_last, p_first) < 0) swap(p_last, p_first, size);
    if (compar(p_last, p_mid) < 0)   swap(p_last, p_mid, size);

    swap(p_mid, p_last, size);
}

/**
 * @brief Lomuto partition scheme using a Median-of-Three pivot.
 * @param base Pointer to the first element.
 * @param nitems Number of elements.
 * @param size Size of each element.
 * @param compar Comparison function.
 * @return Final index of the pivot.
 */
static size_t partition(void *base, size_t nitems, size_t size, CompareFn compar) {
    median_of_three(base, nitems, size, compar);

    char *base_ptr = (char *)base;
    char *pivot = base_ptr + ((nitems - 1) * size);
    size_t i = 0;

    for (size_t j = 0; j < nitems - 1; ++j) {
        char *p_j = base_ptr + (j * size);
        if (compar(p_j, pivot) <= 0) {
            swap(base_ptr + (i * size), p_j, size);
            i++;
        }
    }
    swap(base_ptr + (i * size), pivot, size);
    return i;
}

/**
 * @brief Recursive QuickSort with a fallback to Selection Sort.
 * @param base Pointer to the first element.
 * @param nitems Number of elements.
 * @param size Size of each element.
 * @param k Threshold for switching to Selection Sort.
 * @param compar Comparison function.
 */
static void quick_sort(void *base, size_t nitems, size_t size, size_t k, CompareFn compar) {
    if (nitems < 2) return;

    if (nitems < k) {
        selection_sort(base, nitems, size, compar);
        return;
    }

    size_t pivot_idx = partition(base, nitems, size, compar);

    quick_sort(base, pivot_idx, size, k, compar);

    char *base_ptr = (char *)base;
    if (pivot_idx + 1 < nitems) {
        size_t right_items = nitems - pivot_idx - 1;
        quick_sort(base_ptr + ((pivot_idx + 1) * size), right_items, size, k, compar);
    }
}

/**
 * @brief Public interface for the hybrid sorting algorithm.
 * @param base Pointer to the first element.
 * @param nitems Number of elements.
 * @param size Size of each element.
 * @param k Threshold for switching algorithms.
 * @param compar Comparison function.
 */
void hybridsort(void *base, size_t nitems, size_t size, size_t k, CompareFn compar) {
    if (nitems < 2) return;
    quick_sort(base, nitems, size, k, compar);
}