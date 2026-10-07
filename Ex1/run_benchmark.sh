#!/bin/bash

# Configurazione
INPUT="records.bin"
OUTPUT="/dev/null"
EXECUTABLE="./bin/main_ex1"

K_VALUES=(0 10 50 100 500 1000 5000 10000 1000000)
FIELDS=(1 2 3 4)

echo "Inizio Benchmark Algoritmo Ibrido (Auto-Timeout 5 min)"
echo "==================================================================="

for field in "${FIELDS[@]}"; do
    echo ""
    echo "------------------------------------------------"
    echo " Esecuzione test sul CAMPO $field "
    echo "------------------------------------------------"

    for k in "${K_VALUES[@]}"; do
        echo "Soglia K=$k :"

        # 'timeout 300' interrompe il processo se supera i 300 secondi (5 minuti)
        timeout 300 $EXECUTABLE $INPUT $OUTPUT $field $k

        # Controlliamo se timeout ha bloccato il processo (codice di uscita 124)
        if [ $? -eq 124 ]; then
            echo "TIMEOUT: Esecuzione interrotta (superati i 5 minuti)."
        fi
        echo "-----------------"
    done
done

echo "Benchmark completato su tutti i campi!"