## **Relazione Esercizio 4: Grafo Stradale e Visita BFS** ##
    
1.Obiettivo:
        L'esercizio richiedeva l'implementazione di un grafo per rappresentare i collegamenti tra le località italiane 
        e l'esecuzione di una visita in ampiezza (BFS) per determinare la raggiungibilità dei nodi a partire da una città specifica.
   

2.Scelte Implementative: 
        Struttura del Grafo: È stata utilizzata una Lista di Adiacenza. 
        Questa scelta è ottimale per il dataset fornito (italian_dist_graph.csv) in quanto il grafo stradale è "sparso" (pochi archi per ogni nodo).
        Gestione dei Nomi: Per mappare i nomi delle località ai nodi del grafo è stata integrata la Hash Table sviluppata nell'Esercizio 2, permettendo ricerche e inserimenti in tempo medio costante $O(1)$.
        Algoritmo di Visita: La BFS è stata implementata con l'ausilio di una coda (Queue), garantendo una complessità temporale di $O(V + E)$.
        Archi Non Diretti: Ogni voce del dataset è stata trattata come un arco bidirezionale, inserendo sia il collegamento A-B che quello B-A.

3. Risultati dei test e Analisi Tecnica: 
    | Metrica                 | Valore    | Note                                     |
    |:----------------------- |:----------|:-----------------------------------------|
    | **Unit Tests**          | 5 / 5     | Tutti i test (BFS, Neighbors, etc.) PASS |
    | **Allocazioni Heap**    | 94        | Gestione precisa in ambiente di test     |
    | **Memory Leaks**        | 0 bytes   | Deallocazione totale (Valgrind OK)       |
    | **Errori di Memoria**   | 0         | Nessun accesso invalido o Segfault       |
    | **Nodi Visitati (Main)**| 18.636    | Partendo da "torino"                     |
    | **Stato Consegna**      | Completo  | Requisiti della traccia soddisfatti      |

4. Conclusioni
     Il superamento degli Unit Test e l'assenza di memory leak confermano la robustezza dell'implementazione. 
     L'utilizzo di strutture dati efficienti (Hash Table e Liste di Adiacenza) permette di processare l'intero grafo stradale italiano (oltre 18k nodi) in tempi estremamente ridotti, 
     rispettando i vincoli di complessità computazionale richiesti.