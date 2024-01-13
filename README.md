# Prova finale Algoritmi e Strutture dati, a.a. 2022/2023, Politecnico di Milano
## Descrizione
Il progetto consiste nell'implementazione di un sistema di gestione di un'autostrada con stazioni di servizio dotate di veicoli elettrici a noleggio. L'obiettivo è pianificare il percorso con il minor numero di tappe tra due stazioni specifiche, rispettando vincoli di autonomia e distanza.
L'autostrada è rappresentata come una sequenza di stazioni di servizio. Ogni stazione è identificata da una distanza univoca dall'inizio dell'autostrada. Ogni stazione ha un parco veicoli che comprende al massimo 512 veicoli, ciascuno con un'autonomia espressa in chilometri.  
Per maggiori info vedi foglio specifica.

## Comandi Supportati

Il sistema supporta i seguenti comandi attraverso un file di input:

- **aggiungi-stazione distanza numero-auto autonomia-auto-1 ... autonomia-auto-n**: Aggiunge una stazione con la distanza specificata, il numero di veicoli, e le autonomie di ciascun veicolo. La stazione non viene aggiunta se esiste già una stazione alla stessa distanza.
    
    - Stampa attesa come risposta: "aggiunta" oppure "non aggiunta".
    
- **demolisci-stazione distanza**: Rimuove la stazione alla distanza specificata, se esiste.
    
    - Stampa attesa come risposta: "demolita" oppure "non demolita".
    
- **aggiungi-auto distanza-stazione autonomia-auto-da-aggiungere**: Aggiunge un'auto alla stazione specificata, se esiste.
    
    - Stampa attesa come risposta: "aggiunta" oppure "non aggiunta".
    
- **rottama-auto distanza-stazione autonomia-auto-da-rottamare**: Rottama un'auto dalla stazione specificata, se esiste un'auto con l'autonomia specificata.
    
    - Stampa attesa come risposta: "rottamata" oppure "non rottamata".
    
- **pianifica-percorso distanza-stazione-partenza distanza-stazione-arrivo**: Pianifica il percorso tra due stazioni rispettando i vincoli dati.
    
    - Stampa attesa come risposta: le tappe in ordine di percorrenza, rappresentate con la distanza delle stazioni da inizio autostrada, separate da spazi e alla fine seguite da un a-capo. Se il percorso non esiste, stampa "nessun percorso".

## Valutazione 30 / 30
