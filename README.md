# WordCount

### Introduzione
- Studente: Alessia Natale 
- Matricola: 0522501110
- Istanze AWS: m4.xlarge

## Indice
- [Problema](#problema)
- [Installazione](#installazione)
- [Soluzione proposta](#soluzione-proposta)
  * [Dettagli implementazione](#dettagli-implementazione)
- [Correttezza](#correttezza)
- [Analisi performance](#analisi-performance)
  * [Strong Scalability](#strong-scalability)
  * [Weak Scalability](#weak-scalability)
- [Conclusioni](#conclusioni)

## Problema
Il progetto prevede la realizzazione di un programma che conti il numero di parole all'interno di file utilizzando un approccio parallelo e distribuito attraverso Open MPI.
In particolare il programma è diviso in 3 step:
- il master legge la directory contenente tutti i file di testo e divide i file in modo tale che ogni precesso ne avrà una parte da cui dovrà contare la frequenza con la quale si ripete ogni parola
- i processi inviano le frequenze al master che le combinerà insieme
- il master crea un file csv contenente tutte le parole con le relative frequenze

## Installazione

## Soluzione proposta


### Dettagli implementazione

## Correttezza

## Analisi performance

### Strong Scalability

### Weak Scalability

## Conclusioni
