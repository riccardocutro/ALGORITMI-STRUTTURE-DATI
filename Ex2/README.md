# Relazione Esercizio 2: Hash Table & Frequenza Parole

## 1. Dettagli Implementativi
Per il conteggio delle occorrenze delle parole nel testo (*Iliade*), è stata implementata una **Hash Table** con le seguenti caratteristiche:
* **Gestione delle Collisioni:** Separate Chaining (liste di trabocco). Questa scelta garantisce robustezza anche con load factor elevati, evitando il clustering tipico dell'indirizzamento aperto.
* **Funzione di Hash:** Algoritmo **djb2** (Bernstein). È stata scelta per la sua semplicità ed efficacia sulle stringhe ASCII, garantendo una buona distribuzione uniforme e riducendo le collisioni grazie all'effetto valanga (moltiplicazione per 33).
* **Dynamic Resizing (Espansione e Contrazione):** La tabella gestisce in modo bidirezionale la propria capacità per ottimizzare l'uso della memoria mantenendo una complessità temporale ammortizzata di $O(1)$:
    * **Espansione:** La tabella raddoppia la propria capacità quando il *Load Factor* supera la soglia dello `0.75`.
    * **Contrazione (Shrinking):** Per evitare sprechi di memoria a seguito di molte eliminazioni di elementi, la tabella dimezza la propria capacità quando il *Load Factor* scende al di sotto dello `0.25`. È stato introdotto un limite inferiore (`INITIAL_CAPACITY = 127`) per evitare contrazioni eccessive quando la tabella è quasi vuota.

## 2. Risultati Sperimentali (Dataset: iliade.txt)

Sono stati eseguiti test variando la lunghezza minima della parola (`len_threshold`) per analizzare il comportamento della struttura con carichi di dati differenti.

| Soglia Lunghezza | Parole Totali | Chiavi Univoche | Capacità Tabella | Collisioni | Max Chain | Load Factor |
|:-----------------|:--------------|:----------------|:-----------------|:-----------|:----------|:------------|
| **2 caratteri** | 150.782       | 7.378           | 16.256           | 1.460      | 5         | 0.45        |
| **4 caratteri** | 82.550        | 7.941           | 16.256           | 1.372      | 5         | 0.49        |
| **5 caratteri** | 51.075        | 6.310           | 16.256           | 1.108      | 5         | 0.39        |

**Note sui dati:**
* **Parola più frequente (soglia 4):** 'with' (1.694 occorrenze).
* **Parola più frequente (soglia 5):** 'their'.
* **Check Memoria:** Il test con Valgrind conferma 0 memory leaks e 0 errori di accesso.

## 3. Analisi Critica

### Efficienza della Funzione di Hash (djb2)
I dati confermano l'eccellente qualità della funzione `djb2`.
* **Distribuzione Uniforme:** Anche nel caso più grande (150.000 parole inserite), la catena di collisioni più lunga (*Max Chain*) si ferma a **5 elementi**.
* **Impatto sulle prestazioni:** Una catena massima di 5 nodi implica che, nel caso peggiore assoluto, la ricerca di una parola richiede 5 confronti stringa più il calcolo dell'hash. Questo è computazionalmente trascurabile e conferma che la tabella opera in tempo costante $O(1)$.

### Gestione del Carico e Resizing
Il Load Factor si mantiene stabile tra **0.39** e **0.49**.
Questo indica che la politica di ridimensionamento funziona correttamente: la tabella cresce proattivamente prima di saturarsi. Mantenere il Load Factor sotto 0.75 è cruciale per il *Chaining*, poiché impedisce alle liste di trabocco di allungarsi eccessivamente, preservando le prestazioni di lookup.

### Correttezza del Pre-processing
L'analisi con soglia a 2 caratteri mostra un numero elevato di occorrenze per articoli e preposizioni ("the", "of"), che vengono correttamente filtrati alzando la soglia a 4 o 5. Il sistema gestisce correttamente la pulizia della punteggiatura e la *case-insensitivity*, raggruppando correttamente varianti come "The", "the." e "the".

## Utilizzo di Strumenti di Intelligenza Artificiale Generativa (LLM)

Come richiesto dalla traccia, si documenta qui l'utilizzo di Large Language Models (Gemini) che hanno agito come supporto allo sviluppo e all'analisi.


### Prompt inserito:
Sto lavorando all'Esercizio 2 di Laboratorio di Algoritmi. Devo implementare una Hash Table in C generica (che accetti void* come chiavi) per contare la frequenza delle parole in un file di testo molto lungo. La specifica richiede la gestione delle collisioni tramite Chaining (liste di trabocco)
Vorrei sapere:Q uale funzione di hash mi consigli per minimizzare le collisioni sulle stringhe (djb2? va bene?) Come struttureresti la struct del nodo e della tabella per gestire la genericità dei dati?Qual è la strategia migliore per il resize quando la tabella diventa troppo piena, per mantenere le prestazioni $O(1)$?"
### Aree di Utilizzo

1.  **Scelte Architetturali e Algoritmi:**
    * Si è consultato l'LLM per valutare quale funzione di hash fosse più adatta per un dataset testuale generalista. L'IA ha suggerito l'algoritmo djb2, spiegando che offre un ottimo compromesso tra velocità di calcolo e distribuzione uniforme delle chiavi stringa, riducendo le collisioni rispetto a una somma dei caratteri.
    * Abbiamo discusso la strategia di Resizing: l'IA ha suggerito di raddoppiare la dimensione della tabella quando il Load Factor supera una soglia (0.75) per mantenere la complessità ammortizzata a $O(1)$.

2. **Criticità Riscontrate e Correzioni (Gestione Memoria & Parsing):**
    * **Gestione Memory Leak con Generics:** L'LLM ha inizialmente proposto una gestione della deallocazione logica (solo nodi e array di bucket). Trattandosi di una struttura generica (`void*`), è stato necessario intervenire manualmente nel `main` per la deallocazione esplicita delle chiavi e dei valori (stringhe allocate con `string_dup` e interi), validando l'assenza di leak con Valgrind.
    * **Shrinking e Ottimizzazione Spaziale:** Inizialmente la tabella non si ridimensionava in seguito a cancellazioni. Analizzando con l'LLM il rischio di sprechi di memoria, abbiamo integrato una logica di *shrinking*: la capacità viene dimezzata quando il Load Factor scende $\le 0.25$, mantenendo un limite inferiore (`INITIAL_CAPACITY = 127`) per evitare spreco di memoria.
    * **Risoluzione Bug nei Test (Pointer Trap):** Durante la stesura dei test Unity per lo *shrinking*, l'LLM ha aiutato a diagnosticare un problema critico sui puntatori: usare l'indirizzo dell'indice in un ciclo `for` (`&i`) come chiave causava l'associazione di tutti i nodi alla stessa cella di memoria stack. È stato risolto introducendo un array statico (`static int test_data[1000]`) per garantire indirizzi univoci e permanenti.
    * **Parsing del Testo:** Le soluzioni iniziali proposte dall'IA per leggere le parole dal file (basate su `fscanf` o `strtok`) si sono rivelate inadeguate per una pulizia fine della punteggiatura. Il codice è stato riscritto manualmente carattere per carattere (`fgetc` + `isalnum`) per garantire la corretta normalizzazione dei termini.
3.  **Analisi dei Risultati:**
    * Dopo aver eseguito il codice sul testo `iliade.txt`, abbiamo fornito all'LLM i dati grezzi di output (numero di collisioni, lunghezza massima delle catene, load factor).
    * L'IA ha supportato l'interpretazione critica di questi dati, evidenziando come una catena massima di 5 elementi su 80.000 inserimenti fosse un indicatore di ottima salute della funzione di hash, aiutandoci a formalizzare le conclusioni tecniche presenti nella sezione "Analisi Critica".
### Verifica e Validazione
Tutto il codice suggerito o corretto dall'IA è stato analizzato, compreso e testato manualmente. Le spiegazioni teoriche fornite dal modello sono state verificate confrontandole con le slides del corso.