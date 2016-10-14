# reactivision 1.6

Ad oggi (Settembre 2016) _reacTIVision_ utilizza il protocollo **TUIO 1.1** proprio come la versione _1.5.1_. Implementa però un sistema decisamente più robusto per effettaure il _finger tracking_ e la nuova capacità di effettuare il tracking dei _blob_!

Pur disponibili gli esempi funzionanti di client che implementano il _TUIO2.0_, ancora la versione attuale di _reacTIVision_ non è in grado di comunicare loro.

## reacTIVision: Verbose Output
reacTIVision può essere utilizzato in modalità **verbose** premendo il tasto _V_. A schermo vengono mostrati dei messaggi il cui significato è di seguito velocemente riepilogato (si considerano solo elementi di tipo _object_, non indichiamo qui le informazione che comunque reacTIVision riporta in merito a _finger/cursor_ e _blob_).

Un oggetto viene **aggiunto**:
* Symbol ID
* Session ID
* posizione X 
* posizione Y
* angolo

Un oggetto viene **updatato**:
* Symbol ID
* Session ID
* posizione X 
* posizione Y
* angolo
* velocità X
* velocità Y
* rotation speed
* accelerazione movimento
* accelerazione rotazione

Un oggetto viene **rimosso**:
* Symbol ID
* Session ID

## coordinate normalizzate
Il protocollo TUIO prevede che le coordinate vengano inviate sottoforma di _float 32 bit_. Tali coordinate sono normalizzate in base alla dimensione del sensore. In altri termini, se l'oggetto si trovasse nell'upper-left corner dell'immagine captata dalla telecamera, le sue coordinate trasmesse tramite TUIO risulterebbero (0, 0).

_reacTIVision_ a sua volta usa un sistema di calibrazione, il quale permette alle coordinate degli oggetti individuati dal sensore di essere inviate in modo normalizzato non tanto rispetto alle dimensioni dell'intero sensore, quanto piuttosto a quelle di una sua porzione. 
Risulterà così che, una volta completata la calibrazione - usciti quindi dalla _calibration mode_ di reacTIVision - un oggetto avrà coordinate (0, 0) quando verrà a trovarsi nell'angolo superiore sinistro della griglia di calibrazione!

Attenzione: affinchè le coordinate degli oggetti vengano inviate in modo normalizzato e calibrato occorre uscire dalla modalità di _calibrazione_. Questo risulta evidente se si abilita la modalità _verbose_ di reacTIVision.

## Calibrazione
Per calibrare si deve quindi usare una mappa che riporti la griglia rettangolare e ricordarsi che, come nel nostro caso, se l'applicativo prevede che dell'immagine calibrata se ne usi solo una porzione, moltiplicare e scalare tutto in modo opportuno. 

## BPM
Il valore di BPM viene calcolato ogni volta che il fiducial BPM subisce un aggiornamento. Il valore è ottenuto come mappatura dell'angolo di rotazione del fiducial stesso rispetto ad un range prestabilito di apertura (da 30 a 260 BPM).
Il valore di BPM così calcolato viene usato nelle prime righe del metodo _ofApp::update_ per ricavare la durata della croma in millisecondi (**croma_time**).
Tale valore è importante non soltanto perchè inviato via OSC a PD per temporizzare l'esecuzione musicale del sequencer, ma anche perchè serve per temporizzare l'animazione del fiducial synth la cui figura si espande e si contrae a tempo di musica!


