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
    char *pa = (char *)a;
    char *pb = (char *)b;
    for (size_t i = 0; i < size; ++i) {
        char tmp = pa[i];
        pa[i] = pb[i];
        pb[i] = tmp;
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
 * @brief Choose a random pivot.
 */
static void random_pivot(void *base, size_t nitems, size_t size) {
    char *base_ptr = (char *)base;

    size_t rand_idx = ((size_t)rand() ^ ((size_t)rand() << 15)) % nitems;
    swap(base_ptr + (rand_idx * size), base_ptr + ((nitems - 1) * size), size);
}

/**
 * @brief Recursive QuickSort using 3-Way Partitioning and fallback to Selection Sort.
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

    random_pivot(base, nitems, size);
    char *base_ptr = (char *)base;
    char pivot_val[size];  //temporary buffer
    memcpy(pivot_val, base_ptr + ((nitems - 1) * size), size); //copy of the bytes

    size_t lt = 0;
    size_t unk = 0;
    size_t gt = nitems;

    while (unk < gt) {
        char *curr_elem = base_ptr + (unk * size);
        int cmp_res = compar(curr_elem, pivot_val);

        if (cmp_res < 0) {
            swap(base_ptr + (lt * size), curr_elem, size);
            lt++;
            unk++;
        } else if (cmp_res > 0) {
            gt--;
            swap(curr_elem, base_ptr + (gt * size), size);
        } else {
            unk++;
        }
    }

    if (lt > 0) {
        quick_sort(base, lt, size, k, compar);
    }
    if (nitems > gt) {
        size_t right_items = nitems - gt;
        quick_sort(base_ptr + (gt * size), right_items, size, k, compar);
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