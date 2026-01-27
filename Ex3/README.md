# Relazione Esercizio 3: Gestione Task e Coda di Priorità

## 1. Descrizione dell'Esercizio
Il progetto implementa un sistema di gestione per una coda di task con priorità variabile. 
Il software legge un dataset di un milione di elementi, organizza i task in base alla loro urgenza (priorità minima) e produce un file di output con la sequenza di esecuzione corretta.

## 2. Scelte Progettuali
Per gestire la mole di dati ($10^6$ elementi), sono state scelte strutture dati con prestazioni ottimali:
- **Min-Heap (Priority Queue):** Permette l'estrazione del task prioritario in tempo logaritmico $O(\log N)$.
- **Hash Table:** Utilizzata per il mapping dei nomi dei task, garantendo accesso $O(1)$.

## 3. Risultati Sperimentali
I test condotti con il dataset fornito hanno prodotto i seguenti dati tecnici:

| Metrica               | Valore                      |
|:----------------------|:----------------------------|
| **File di Input**     | `tasks.csv`                 |
| **File di Output**    | `output.csv`                |
| **N. di Allocazioni** | 2.998.042                   |
| **Memoria Totale**    | 68,80 MB                    |
| **Errori Memoria**    | Nessuno (0 errori Valgrind) |
| **Stato Leak**        | 0 bytes persi               |

## 4. Conclusioni
L'analisi con Valgrind conferma la robustezza dell'implementazione: 
nonostante l'elevato numero di operazioni di allocazione e rimozione (circa 3 milioni), il programma gestisce la memoria in modo impeccabile. 
Le prestazioni temporali risultano coerenti con la complessità teorica attesa, rendendo il sistema scalabile per dataset di grandi dimensioni.