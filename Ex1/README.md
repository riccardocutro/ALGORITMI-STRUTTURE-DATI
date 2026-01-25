# Relazione Esercizio 1: QuickSort Ibrido

## Descrizione dell'algoritmo
L'algoritmo implementato è un QuickSort ibrido. La strategia consiste nell'utilizzare il QuickSort per la partizione principale dell'array e passare al Selection Sort quando la dimensione del sotto-array scende sotto una soglia $K$.
Per la scelta del pivot è stata utilizzata la tecnica della **Mediana di Tre** per evitare il caso peggiore su array già ordinati.

## Risultati dei Test
I test sono stati condotti su un dataset di 20.000.000 di record. Il tempo massimo per ogni test è stato fissato a 5 minuti.

| K (Soglia)              | Campo 1 (ID)      | Campo 2 (Float)     | Campo 3 (Int) | Campo 4 (String) |
|:------------------------|:------------------|:--------------------|:--------------| :--- |
| **0 (QuickSort)**       | 4.670880 s        | 6.374291 s          | 7.818063 s    | 1553.25 s |
| **10**                  | 1.812429 s        | 6.369347 s          | 5.122416 s    | TIMEOUT |
| **50**                  | 1.736389 s        | 5.990537 s          | 5.182632 s    | TIMEOUT |
| **100**                 | 2.688770 s        | 6.675819 s          | 5.625639 s    | TIMEOUT |
| **500**                 | 8.939471 s        | 15.632720 s         | 18.967284 s   | TIMEOUT |
| **1000**                | 17.707014 s       | 27.287280 s         | 24.810909  s  | TIMEOUT |
| **5000**                | 129.932205 s      | 116.077060 s        | 115.835979  s | TIMEOUT |
| **10000**               | 267.422354 s      | 255.250006 s        | 226.661379  s | TIMEOUT |
| **1000000**             | TIMEOUT (> 5 min) | TIMEOUT (> 5 min) s | TERMINATED  s | TIMEOUT |

## Analisi Critica
Dall'analisi dei tempi di esecuzione su un dataset di 20 milioni di record, si possono trarre le seguenti conclusioni tecniche:
L'efficacia dell'ibridazione (Il "Sweet Spot"). 
I dati confermano che l'algoritmo ibrido è più efficiente del QuickSort puro ($K=0$).
Per i campi numerici (ID, Float, Int), il tempo di esecuzione cala sensibilmente passando da $K=0$ a $K=10$ o $K=50$.
Perché accade? Il Selection Sort, pur essendo un algoritmo quadratico $O(n^2)$, su array molto piccoli è più veloce del QuickSort perché non ha l'overhead delle chiamate ricorsive e della gestione dello stack. 
Il punto di equilibrio ottimale nel mio test si colloca tra $K=10$ e $K=50$.2. 
Il decadimento prestazionale (Il "Muro Quadratico")Superata la soglia di $K=100$, 
si nota un drastico peggioramento delle prestazioni:Con $K=5000$, il tempo sale a circa 115-130 secondi.Con $K=10000$, 
il tempo raddoppia quasi perfettamente (circa 230-260 secondi). 
Questo andamento quasi lineare del peggioramento (all'aumentare di $K$) riflette il fatto che stiamo costringendo l'algoritmo a usare un metodo $O(n^2)$ su porzioni di dati troppo grandi, vanificando i vantaggi del QuickSort. 
Il caso critico: Le Stringhe (Campo 4): Il Campo 4 rappresenta lo scenario peggiore. 
Mentre i numeri vengono confrontati quasi istantaneamente dalla CPU, il confronto tra stringhe (strcmp o strncmp) è estremamente oneroso.
Il QuickSort puro riesce a completare l'operazione in 1553 secondi (circa 25 minuti), ma l'inserimento di qualsiasi soglia di Selection Sort ($K>0$) porta immediatamente al TIMEOUT.Questo accade perché il Selection Sort effettua un numero di confronti pari a $O(n^2)$ 
e quando il costo di ogni singolo confronto è alto (come nelle stringhe), l'algoritmo diventa impraticabile.
Robustezza del Pivot (Mediana di Tre): Il Campo 1 (ID) è pre-ordinato. In un QuickSort ingenuo (che sceglie il primo elemento come pivot), questo causerebbe un tempo di esecuzione pessimo.
I risultati (circa 1.7s - 1.8s per $K$ bassi) dimostrano che la strategia della Mediana di Tre funziona correttamente, garantendo una partizione bilanciata anche su dati già ordinati.

## Risultati del Test EX2 (iliade.txt)
Il test è stato eseguito con una lunghezza minima di **4 caratteri**.

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