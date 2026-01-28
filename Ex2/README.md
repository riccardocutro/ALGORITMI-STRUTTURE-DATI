## Risultati del Test EX2 (iliade.txt)
**Il primo test è stato eseguito con una lunghezza minima di **4 caratteri**.**

### Risultati Generali
- **Parole valide processate:** 82.550
- **Chiavi univoche memorizzate:** 7.941
- **Parola più frequente:** 'with' (1.694 occorrenze)

### Statistiche Tecniche della Hash Table
| Metrica               | Valore | Note                                     |
|:----------------------| :----- | :--------------------------------------- |
| **Capacità Totale**   | 16.256 | Risultato di vari raddoppi (resizing)    |
| **Collisioni Totali** | 1.682  | Estremamente basse rispetto alle chiavi  |
| **Catena più lunga**  | 5      | Ottimo: garantisce prestazioni $O(1)$    |
| **Load Factor**       | 0.49   | Sotto la soglia critica di 0.75          |

## Analisi Critica
I dati mostrano che la funzione di hash **djb2** distribuisce le stringhe in modo eccellente.
Anche con un dataset di oltre 80.000 parole, la struttura non degrada: la catena di collisioni massima (5) è trascurabile,
mantenendo l'accesso alla tabella costante.
Il meccanismo di **Dynamic Resizing** ha funzionato correttamente,
stabilizzandosi a una capacità di 16.256 bucket per mantenere il Load Factor vicino al 50%.

**Analisi del Test con Soglia di Lunghezza = 2**
Oltre al test standard (4 lettere), è stata effettuata una prova abbassando la soglia a 2 lettere. 
Questo ha permesso di osservare il comportamento della struttura dati con un volume di input maggiore.

| Metrica                 | Valore    | Note                                     |
|:------------------------|:----------|:-----------------------------------------|
| **Parole Processate**   | 150.782   | Include articoli e preposizioni (es. the)|
| **Capacità Totale**     | 16.256    | Risultato di vari raddoppi (resizing)     |
| **Chiavi Uniche**       | 7.378     | Vocabolario distinto memorizzato         |
| **Collisioni Totali**   | 1.460     | Estremamente basse grazie a djb2         |
| **Catena più lunga**    | 5         | Ottimo: garantisce prestazioni $O(1)$    |
| **Load Factor**         | 0.45      | Sotto la soglia critica di 0.75          |

Considerazioni Tecniche emerse dal Log: 
Efficienza della Funzione di Hash (djb2): 
Nonostante l'aggiunta di migliaia di parole brevi (anche molto simili tra loro, come "the", "thy", "this"), 
la Longest Chain è rimasta ferma a 5. Questo conferma che la funzione di hash distribuisce le chiavi in modo quasi uniforme, 
evitando degradazioni delle prestazioni verso il caso pessimo $O(n)$.
Gestione del Carico: Con 150.000 parole processate, la tabella ha mantenuto un Load Factor di 0.45. 
Questo dimostra che il meccanismo di Dynamic Resizing ha scalato correttamente la capacità fino a 16.256 bucket, garantendo spazio sufficiente per evitare un eccesso di collisioni.

Punteggiatura e Case-Sensitivity: 
Il conteggio di "the" (oltre 10.000) conferma che la pulizia della punteggiatura e la conversione in minuscolo funzionano correttamente, raggruppando varianti che altrimenti verrebbero contate come parole diverse (es: "The", "the.", "the!").

**Analisi del Test con soglia di Lunghezza = 5.**

| Metrica               | Valore | Note                                     |
|:----------------------|:-------|:-----------------------------------------|
| **Parole Processate** | 51.075 | Filtrate con soglia minima di 5 lettere  |
| **Parola più comune** | 'their'| 872 occorrenze totali                    |
| **Capacità Totale**   | 16.256 | Risultato di vari raddoppi (resizing)    |
| **Chiavi Uniche**     | 6.310  | Parole distinte memorizzate nella tabella|
| **Collisioni Totali** | 1.108  | Molto basse rispetto al numero di chiavi |
| **Catena più lunga**  | 5      | Ottimo: garantisce prestazioni $O(1)$    |
| **Load Factor**       | 0.39   | Ben al di sotto della soglia di stress   |

**Analisi dei Risultati**
L'analisi con soglia fissata a 5 lettere mostra come il "rumore" delle parole comuni (articoli e preposizioni come the, and, with) venga eliminato, facendo emergere termini più significativi come 'their'.
Dal punto di vista computazionale, i dati tecnici confermano l'efficienza della struttura:
Distribuzione: Nonostante oltre 6.000 chiavi uniche, la catena di collisione più lunga è di soli 5 elementi, confermando che la funzione di hash distribuisce i dati in modo uniforme.
Efficienza: Un Load Factor di 0.39 indica che la tabella è molto veloce nelle operazioni di ricerca e inserimento, poiché circa il 60% della memoria allocata rimane disponibile per gestire eventuali picchi di collisioni.
Robustezza: Il sistema è stato verificato con Valgrind, riportando 0 leak e 0 errori, confermando la corretta gestione dei puntatori durante il resizing e la deallocazione finale.