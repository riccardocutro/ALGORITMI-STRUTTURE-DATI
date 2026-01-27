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