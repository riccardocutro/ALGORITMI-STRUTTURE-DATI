#include "sort.h"
#include <stdlib.h>
#include <string.h>

/* --- Funzioni Ausiliarie Interne (static) --- */

/**
 * Scambia due elementi di dimensione 'size'.
 * Usa un buffer statico per performance su elementi piccoli (Record è 36 byte).
 */
static void swap(void *a, void *b, size_t size) {
    if (a == b) return;

    char buffer[256]; // Buffer statico veloce
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
 * Selection Sort: O(n^2). Efficiente su array piccoli.
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
 * Calcola il pivot usando la tecnica "Median-of-Three".
 * Prende il primo, il centrale e l'ultimo elemento, li ordina e usa il mediano come pivot.
 */
static void median_of_three(void *base, size_t nitems, size_t size, CompareFn compar) {
    char *base_ptr = (char *)base;
    size_t mid = (nitems / 2);

    char *p_first = base_ptr;
    char *p_mid   = base_ptr + (mid * size);
    char *p_last  = base_ptr + ((nitems - 1) * size);

    // Ordina low, mid, high in modo che mid sia il mediano
    if (compar(p_mid, p_first) < 0) swap(p_mid, p_first, size);
    if (compar(p_last, p_first) < 0) swap(p_last, p_first, size);
    if (compar(p_last, p_mid) < 0)   swap(p_last, p_mid, size);

    // Mettiamo il pivot (che ora è in p_mid) in fondo per il partizionamento
    swap(p_mid, p_last, size);
}

/**
 * Partition Lomuto (Standard) con pivot scelto via Median-of-Three
 */
static size_t partition(void *base, size_t nitems, size_t size, CompareFn compar) {
    // 1. Scegli pivot robusto e spostalo in fondo
    median_of_three(base, nitems, size, compar);

    char *base_ptr = (char *)base;
    char *pivot = base_ptr + ((nitems - 1) * size); // Il pivot è in fondo
    size_t i = 0;

    for (size_t j = 0; j < nitems - 1; ++j) {
        char *p_j = base_ptr + (j * size);
        if (compar(p_j, pivot) <= 0) {
            swap(base_ptr + (i * size), p_j, size);
            i++;
        }
    }
    // Metti il pivot nella sua posizione finale (i)
    swap(base_ptr + (i * size), pivot, size);
    return i;
}

/**
 * QuickSort Ricorsivo con check Ibrido
 */
static void quick_sort(void *base, size_t nitems, size_t size, size_t k, CompareFn compar) {
    // Caso base
    if (nitems < 2) return;

    // Strategia Ibrida: se sotto soglia k, usa SelectionSort
    if (nitems < k) {
        selection_sort(base, nitems, size, compar);
        return;
    }

    // Altrimenti: Partitioning e Ricorsione
    size_t pivot_idx = partition(base, nitems, size, compar);

    // Ricorsione sinistra
    quick_sort(base, pivot_idx, size, k, compar);

    // Ricorsione destra
    char *base_ptr = (char *)base;
    if (pivot_idx + 1 < nitems) {
        size_t right_items = nitems - pivot_idx - 1;
        quick_sort(base_ptr + ((pivot_idx + 1) * size), right_items, size, k, compar);
    }
}

void hybridsort(void *base, size_t nitems, size_t size, size_t k, CompareFn compar) {
    if (nitems < 2) return;
    quick_sort(base, nitems, size, k, compar);
}