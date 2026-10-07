# Relazione Esercizio 1: QuickSort Ibrido

## Descrizione dell'algoritmo
L'algoritmo implementato è un QuickSort ibrido. La strategia consiste nell'utilizzare il QuickSort per la partizione principale dell'array e passare al Selection Sort quando la dimensione del sotto-array scende sotto una soglia $K$.

Nello studio delle prestazioni sono state sviluppate e confrontate due versioni:
1. **Implementazione Iniziale (Schema di Lomuto):** Sfrutta il partizionamento classico di Lomuto abbinato alla scelta del pivot tramite la **Mediana di Tre**.
2. **Implementazione Finale (3 Vie):** Sfrutta un partizionamento a tre vie per raggruppare separatamente gli elementi uguali al pivot, combinato con un **Pivot Randomizzato**.

## Risultati dei Test
I test sono stati condotti su un dataset di 20.000.000 di record. Il tempo massimo per ogni test è stato fissato a 5 minuti (300 secondi).

### Tabella 1: Risultati con lo Schema di Lomuto (Iniziale)

| K (Soglia)              | Campo 1 (ID)      | Campo 2 (Float)     | Campo 3 (Int) | Campo 4 (String) |
|:------------------------|:------------------|:--------------------|:--------------| :--- |
| **0 (QuickSort)** | 4.670880 s        | 6.374291 s          | 7.818063 s    | 1553.25 s |
| **10** | 1.812429 s        | 6.369347 s          | 5.122416 s    | TIMEOUT |
| **50** | 1.736389 s        | 5.990537 s          | 5.182632 s    | TIMEOUT |
| **100** | 2.688770 s        | 6.675819 s          | 5.625639 s    | TIMEOUT |
| **500** | 8.939471 s        | 15.632720 s         | 18.967284 s   | TIMEOUT |
| **1000** | 17.707014 s       | 27.287280 s         | 24.810909  s  | TIMEOUT |
| **5000** | 129.932205 s      | 116.077060 s        | 115.835979  s | TIMEOUT |
| **10000** | 267.422354 s      | 255.250006 s        | 226.661379  s | TIMEOUT |
| **1000000** | TIMEOUT (> 5 min) | TIMEOUT (> 5 min) s | TERMINATED  s | TIMEOUT |

### Tabella 2: Risultati con il Partizionamento a 3 Vie e Pivot Randomico (Finale)

| K (Soglia)        | Campo 1 (ID) | Campo 2 (Float) | Campo 3 (Int) | Campo 4 (String) |
|:------------------|:-------------|:----------------|:--------------|:-----------------|
| **0 (Puro QS)** | 8.721513 s   | 11.462512 s     | 10.701883 s   | 5.398104 s       |
| **10** | 7.880180 s   | 11.459629 s     | 10.600006 s   | 5.211151 s       |
| **50** | 12.135631 s  | 11.374014 s     | 10.837431 s   | 5.274682 s       |
| **100** | 11.054674 s  | 11.678819 s     | 11.905874 s   | 5.344918 s       |
| **500** | 25.904705 s  | 20.522522 s     | 18.349633 s   | 5.444293 s       |
| **1000** | 30.286359 s  | 30.727004 s     | 27.950986 s   | 5.259442 s       |
| **5000** | 116.919855 s | 120.371425 s    | 108.813150 s  | 5.096090 s       |
| **10000** | 223.531456 s | 232.744770 s    | 210.612618 s  | 5.145514 s       |
| **1000000** | TIMEOUT      | TIMEOUT         | TIMEOUT       | TIMEOUT          |

## Analisi dei Risultati

Dai test emergono dei punti fondamentali:

1.  **Ottimizzazione sui numeri (K piccolo):** In entrambe le tabelle, impostando un valore di $K$ basso, l'approccio ibrido si dimostra più veloce del rispettivo QuickSort puro ($K=0$). Questo succede perché su poche righe di dati il Selection Sort agisce in modo puramente iterativo senza gravare sullo stack, abbattendo l'overhead dovuto alle chiamate ricorsive.

2.  **Rallentamento con K grandi:** Se impostiamo $K$ troppo alto, le prestazioni crollano bruscamente in tutte le configurazioni (fatta eccezione per le stringhe a 3-way). Questo perché il Selection Sort esegue ciecamante tutti i confronti possibili.

3. **Coerenza con la teoria della complessità:**
* **Degrado su K grandi:** Confermato dall'esito comune del valore $K=1.000.000$, che subisce sistematicamente un TIMEOUT. Il Selection Sort non è scalabile su grandi moli di dati.

4. **Intervallo ottimale di K:**
* **Campi Numerici:** Il valore ottimale si attesta stabilmente nell'intervallo **tra 10 e 50**. In questo range è presente il perfetto punto di equilibrio tra il risparmio della gestione dello stack.
* **Campo Stringhe (3 Vie):** L'intervallo ottimale si estende notevolmente, registrando ottimi tempi fino a $K=10000$ (5.14s) grazie alla massiccia rimozione preventiva dei duplicati operata dal partizionamento a 3 vie.

5. **L'approccio ibrido offre vantaggi rispetto agli algoritmi puri, ma con riserva:**
* **Vantaggi:** L'ibridazione riduce il tempo complessivo di calcolo a patto di calibrare correttamente il valore soglia.
* **Costo della robustezza:** Si nota che l'introduzione della logica a 3 vie e dello spostamento casuale del pivot (Tabella 2) alza la base dei tempi sui campi numerici stabili rispetto alla semplicità dello schema deterministico di Lomuto (Tabella 1). Tuttavia, questo incremento minimo è il prezzo necessario per garantire una stabilità prestazionale universale ed eludere i casi peggiori $O(n^2)$.

6. **Il confronto critico fondamentale: Campo Stringhe (Lomuto vs 3-Way):**
* **Con lo Schema di Lomuto:** Il Campo 4 collassa costantemente in TIMEOUT a partire da $K=10$ (e richiede ben 1553 secondi nel caso puro). Questo avviene perché il dataset di stringhe (estratto dalla Divina Commedia) contiene un numero elevatissimo di **elementi duplicati**. Lomuto, in presenza di forti duplicati, sbilancia le partizioni degradando al caso pessimo $O(n^2)$. Unito alla pesantezza della funzione `strcmp`, il sistema esaurisce il tempo limite.
* **Con il Partizionamento a 3 Vie:** Il Campo 4 diventa in assoluto il campo più veloce da ordinare, assestandosi su una media eccezionale di **~5.2 secondi** costanti. L'algoritmo *Dutch National Flag* isola gli elementi uguali al pivot al centro dell'array, escludendoli dalle chiamate ricorsive successive. Lo scarto immediato dei duplicati abbatte il numero di confronti totali e di chiamate a `strcmp`, stravolgendo positivamente i tempi di elaborazione.