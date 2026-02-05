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

## Analisi dei Risultati

Dai test emergono dei punti fondamentali:

1.  **Ottimizzazione sui numeri (K piccolo):** Con un valore di $K$ basso (tra 10 e 50), l'algoritmo è più veloce del QuickSort puro. Questo succede perché su pochi elementi il Selection Sort è molto semplice e immediato, mentre il QuickSort perde tempo a gestire le chiamate ricorsive.

2.  **Rallentamento con K grandi:** Se impostiamo $K$ troppo alto (es. 10.000), il programma diventa lentissimo. Il motivo è che il Selection Sort è un algoritmo "quadratico" ($O(N^2)$) e, a differenza di altri algoritmi, esegue **sempre** tutti i confronti possibili, anche se i dati sono quasi ordinati. Su grandi quantità di dati, questo lavoro inutile pesa tantissimo.

3. I risultati sono coerenti con la teoria della complessità:
* **Degrado su K grandi:** Aumentando $K$, il termine quadratico del Selection Sort inizia a dominare, peggiorando drasticamente le prestazioni rispetto al QuickSort puro ($K=0$). Questo conferma che il Selection Sort non è adatto a ordinare sequenze lunghe.

4. Dai test emerge che il valore ottimale di $K$ si attesta nell'intervallo **tra 20 e 50**.
* **Perché:** In questo range si trova il punto di equilibrio dove il risparmio sulle operazioni di gestione dello stack e la migliore località della cache del Selection Sort superano il costo dei confronti aggiuntivi dovuti alla sua natura quadratica.
* Sotto questa soglia ($K < 10$), l'overhead della ricorsione è ancora presente.
* Sopra questa soglia ($K > 100$), l'algoritmo diventa inefficiente a causa del numero eccessivo di confronti ($K^2/2$) effettuati dal Selection Sort.

5. L'approccio ibrido offre vantaggi rispetto agli algoritmi puri, ma con riserva:
* **Vantaggi:** Sui campi numerici (Interi, Float), l'approccio ibrido ha mostrato tempi inferiori rispetto al QuickSort puro ($K=0$), riducendo il tempo totale di esecuzione grazie alla gestione iterativa dei sotto-array piccoli.
* **Limiti:** Il vantaggio è strettamente legato alla scelta di un $K$ corretto. Se $K$ è errato, le prestazioni degradano velocemente al di sotto di quelle dell'algoritmo puro.

6. SI nota una differenza critica tra i campi numerici e il campo stringa:
* **Numeri (ID, Int, Float):** I confronti sono istruzioni CPU native velocissime. Qui l'ibridazione funziona bene.
* **Stringhe:** L'algoritmo soffre enormemente. Il confronto con strcmp è costoso. Poiché il **Selection Sort** esegue ciecamante tutti i confronti possibili per trovare il minimo, il numero totale di chiamate a `strcmp` esplode. Questo spiega perché con le stringhe, anche per $K$ bassi, si rischia il timeout o si hanno prestazioni peggiori rispetto al QuickSort puro (che riesce a scartare dati più velocemente).