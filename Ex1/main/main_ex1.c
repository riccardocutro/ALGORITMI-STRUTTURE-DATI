#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"
#include "sort.h"

void print_usage(const char *prog_name) {
    fprintf(stderr, "Usage: %s <input_file> <output_file> <field_id> <k>\n", prog_name);
    fprintf(stderr, "Fields: 1=ID, 2=Field1, 3=Field2, 4=Field3\n");
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    const char *infile_path = argv[1];
    const char *outfile_path = argv[2];
    int field = atoi(argv[3]);
    size_t k = (size_t)atoi(argv[4]);

    CompareFn compar = NULL;
    switch (field) {
        case 1: compar = compare_id; break;
        case 2: compar = compare_field1; break;
        case 3: compar = compare_field2; break;
        case 4: compar = compare_field3; break;
        default:
            fprintf(stderr, "Errore: Campo '%d' non valido (1-4).\n", field);
            return EXIT_FAILURE;
    }

    printf("Caricamento dati da '%s'...\n", infile_path);
    FILE *fin = fopen(infile_path, "rb");
    if (!fin) {
        perror("Errore apertura file input");
        return EXIT_FAILURE;
    }

    Record *records = NULL;
    size_t n_records = 0;
    
    if (load_records_from_file(fin, &records, &n_records) != 0) {
        fprintf(stderr, "Errore nel caricamento dei record o memoria insufficiente.\n");
        fclose(fin);
        return EXIT_FAILURE;
    }
    fclose(fin);

    printf("Caricati %zu record. Avvio ordinamento (k=%zu)...\n", n_records, k);

    clock_t start = clock();
    
    hybridsort(records, n_records, sizeof(Record), k, compar);
    
    clock_t end = clock();
    double elapsed_seconds = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Ordinamento completato in %.6f secondi.\n", elapsed_seconds);

    printf("Salvataggio su '%s'...\n", outfile_path);
    FILE *fout = fopen(outfile_path, "wb");
    if (!fout) {
        perror("Errore apertura file output");
        free(records);
        return EXIT_FAILURE;
    }

    if (save_records_to_file(fout, records, n_records) != 0) {
        fprintf(stderr, "Errore nella scrittura del file di output.\n");
        fclose(fout);
        free(records);
        return EXIT_FAILURE;
    }

    fclose(fout);
    free(records);

    printf("Operazione completata con successo.\n");
    return EXIT_SUCCESS;
}