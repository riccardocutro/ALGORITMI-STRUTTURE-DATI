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

## Analisi dei Risultati (Dataset: 20M record)

Dai test effettuati emergono tre comportamenti principali:

1.  **Scelta di K (Insertion Sort):** Per i campi numerici, l'algoritmo ibrido è più veloce del QuickSort puro ($K=0$). I tempi migliori si ottengono con $K$ tra 10 e 50, dove il costo dell'Insertion Sort su piccoli array è inferiore all'overhead delle chiamate ricorsive.
2.  **Degrado delle prestazioni:** Aumentando $K$ oltre 100, i tempi peggiorano drasticamente (con $K=10000$ i tempi raddoppiano rispetto all'ottimo), perché su porzioni così grandi la complessità quadratica dell'Insertion Sort diventa dominante.
3.  **Il caso delle Stringhe:** Sul campo stringa, il confronto (`strcmp`) è molto lento. Qui l'ibridazione è controproducente: qualsiasi $K>0$ porta spesso al timeout, mentre il QuickSort puro riesce a terminare (anche se impiega molto tempo), suggerendo che per confronti costosi è meglio minimizzare il numero assoluto di operazioni.
4.  **Pivot:** L'uso della "mediana di tre" ha evitato il caso pessimo sul campo ID (già ordinato), mantenendo i tempi in linea con gli altri campi numerici ($~1.7s$).