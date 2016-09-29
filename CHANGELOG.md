# VIDEOTAVOLO - CHANGELOG

## v1.6 (Sept 2016)
* commentate tutte le occorrenze di metodi, vartiabile, direttive pre il precompilatore associate all'addon **ofxPD** di _David "danomatika" Wilcox_;
* tolti i commenti di intestazione su tutti i file;
* risolti i _warning_ relativi all'uso di metodi deprecati come _ofEllipse_, _ofCircle_, _ofRect_ o _ofLine_, sostituiti invece dai nuovi _ofDrawEllipse_, _ofDrawCircle_, _ofDrawRectangle_ e _ofDrawLine_;


## videotavolo-v1.5.2

### SOLVED - issue07
Se viene fatto un remove e, subito dopo un erase dalla lista, l'animazione relativa al FADE OUT del fiducial non potra' essere apprezzata (controlla sezione 'OBJECT REMOVED' di testApp.cpp). 
Eliminare un oggetto da un vettore significa invocare contestualmente il distruttore? 
Questo dipende. Se il vettore è un contenitore di oggetti, allora eliminare un elemento del vettore significa chimarne il distruttore. Se invece il vettore è una contenitore di puntatori ad oggetti, 
eliminare un elemento dal vettore cancella semplicemente il puntatore ad uno specifico oggetto che però rimane in vita e, avendone noi perso ogni riferimento, non potrà più essere raggiungibile
causando quindi un MEMORY LEAK.
Per questo, nel caso di un vettore di puntatori ad oggetti, occorre invocare esplicitamente anche il distruttore dell'oggetto (sfruttandone il puntatore memorizzato nell'array) prima di eliminare il puntatore.

Risolte alcuni piccoli problemini relativi alla resa grafica degli oggetti nei momenti in cui avviene il FADE_IN e il FADE_OUT
- trasparenza dei contorni esterni neri
- uso della varibile 'A' per BASS e SYNTH in modo tale da scalare come 'transparency' l'ampiezza dei relativi fiducial nelle animazioni FADE_IN FADE_OUT 
Migliorato il modo in cui il fudicial del synth viene animato nel momento in cui viene posizionato sulla superficie. Si evitano così dei bruschi scatti di ampiezza.

### TODO
- ci sarebbe forse da fare un controllo sul management dei messaggi OSC per capire se ci sia qualche cosa che deve essere corretto nel programma (ad esempio un bel 'clear' in un qualcghe punto strategico del programma).
- basare le animazioni non sul FPS ma sul tempo che scorre: si incappera' meno in problemi legati alla variazione dell'FPS. (vedi BlueBook pg 158)
- modifica codice con funzione di callback per il cambio di BPM tra fiducial rotativo e fiducial Synth
- capisci meglio il funzionamento della fequenza proiettore / frequenza monitor / frequenza telecamera / frequenza oF
- utilizzo dei membri statici delle classi per tenere conto di ad esempio numero di istanze presenti vedi Ch 12 C++ ref.
- metti in line alcune funzioni per risparmiare tempo
- utilizza la direttiva #undef per togliere la definizione alle macro nei punti in cui esse non servano più per lasciarle definite solo nelle porzioni di codice dove esse servano (vedi pg. 246 cpp)
- tradurre la matrice multidimensionale TABLE in 2 differenti tabelle: una monodimensionale 3 x 8 x 1 di unsigned int per conservare i SID; l'altra tridimensionale (?) di 3 piani da 3 byte (3 char) l'uno in cui ciascun bit rappresenta lo stato dello strumento in quel particolare istante di tempo.
- provare  a sfruttare l'opzione dei costruttori con valori impostati di default in modo tale da risparmiarsi la necessita' di usare un costruttore vuoto e poi dover chiamare il metodo INIT.

## videotavolo-v1.5.1

### SOLVED - issue 05
Capire perche' una volta inserito il fiducial del basso si verifica un rallentamento nell'esecuzione del programma per non dire, in certi casi, un vero e proprio stallo di alcuni secondi. 
Sembra che il problema descritto nella issue05 sia legato alla quantita' di tempo che trascorre tra un posizionamento e a l'altro del fiducial basso. Ad esempio e' capitato di notare che il tempo di interruzione nel corretto funzionamento della applicazione e' tanto più lungo quanto più lungo e' 
il tempo che trascorre prima di riposizionare il fiducial del basso sulla superficie dall'ultima volta che lo si era rimosso.
Questo potrebbe essere dovuto al fatto che in questo tempo di pausa si accumulano dei messaggi OSC nella coda OF. Occorre limitare l'accumulo dei messaggi OSC nella coda quando il fiducial del basso non e' presente sulla superficie.
Questo potrebbe essere risolto evitando che Pure Data invii messaggi quando non necessario (quando il messaggio e' 0, oppure non c'e' audio per un po' di tempo).

Il problema associato descritto in parte dalla issue05 e' stato risolto.
PD invia messaggi OSC ad OF. Ricordiamo che i messaggi OSC che PD invia a OF sono messaggi che descrivono il livello sonoro in dB dello strumento 'bass'.
ad ogni ciclo di UPDATE, OF analizza i messaggi OSC ancora in attesa d'essere elaborati, in modo da provvedere all'animazione del fiducia 'Bass'.
I messaggi OSC in attesa d'essere elaborati attendono che OF provveda ad analizzarli all'interno di una CODA (vedi classe ofxOscReceiver). Una volta che OF provvede ad analizzarli, questi messaggi vengono eliminati dalla coda.

Il problema lamentato con la issue05 era legato ad una serie di diverse problematiche legate proprio all'OSC e a questa coda.
Dal momento che la coda viene analizzata in OF facendo uso di un ciclo WHILE, ne risultava che, ad ogni UPDATE il programma rimaneva bloccato in questa fase nel compito di esaurire l'intera coda di messaggi qui accumulatisi prima di poter procedere. Il problema non e' visibile se si lascia il fiducia del basso sempre presente sul tavolo di gioco. Questo fa sì che comunque, ad ogni update, i pochi messaggi accumulatisi nel coda dal ciclo precedente venissero processati ad ogni ciclo. Quando però il fiducia del basso veniva tolto dalla superficie, i messaggi si accumulavano nella coda (PuraData continua infatti a mandare messaggi ininterrottamente). Non essendoci alcun fiducial preposto a processarli, nessun messaggio osc veniva analizzato ed eliminato dalla coda.

Quando poi, trascorso un po' di tempo, il fiducia del basso veniva appoggiato nuovamente sulla superficie, il programma si bloccava all'interno del ciclo WHILE proprio per esaminare tutti i messaggi accumulatisi nella coda fino a quel momento. Maggiore il tempo trascorso dall'ultimo uso del basso, maggiore la dimensione della coda da esaurire.

Come abbiamo risolto? abbiamo cominciato ad analizzare il modo in cui PureData invia i messaggi OSC. Innanzitutto abbiamo uniformato i metodi preposti al trattamento dei messaggi facendo uso della sola libreria JDL implementata nel core.
Ricordiamo ancora una volta che i messaggi OSC che PD invia a OF sono messaggi che descrivono il livello sonoro in dB dello strumento 'bass'.
Questi messaggi vengono mandati di continuo anche quando il livello del basso e' NON udibile (livello = 0).
E' proprio di questi messaggi (livello = 0) che la coda dell'oggetto osxOscReceiver si riempiva tra l'uso di un fiducia 'bass' e l'ultimo. Una moltitudine di messaggi di valore nullo intasavano la coda.
Come fare per evitarlo?

* 1) ridurre la frequenza con cui PD invia messaggi OSC a OF. Per poterlo fare occorre sincronizzare l'invio di messaggi OSC su base del valore FPS utilizzato da OF.
Per questo si sfrutta il messaggio OSC '/transport/fps' gia' inviato da OF a PD proprio per questo genere di scopo. In coda analizziamo una problematica secondaria derivata dalla sincronizzazione dei due software.
* 2) evitare che PureData invii messaggi quando non necessario. Per evitare che, comunque, nella coda si accumulino messaggi il cui significato e' superfluo occorre trovare un sistema per inviare messaggi alo quando il livello audio e' diverso da zero. Per meglio dire, occorre inviare messaggi OSC solo quando il livello audio del basso subisce un cambiamento rispetto al livello sonoro precedente. Se il livello rimane costante a zero, una volta inviato un solo messaggio iniziale, non invierò più messaggi fino alla prossima variazione.
Per questo si fa uso dell'oggetto 'chance' il cui scopo e' proprio quello di eliminare la ridondanza in uno stream di numeri.

Attraverso gli aggiustamenti 1) e 2), entrambi applicati al lato server (su PD inteso come mittente dei messaggi OSC verso OF) si e' riusciti a risolvere il problema legato alla issue05.

Come anticipato, l'aggiustamento 1) ci fa scontrare con una ulteriore problematica relativa alla corretta sincronizzazione tra i due programmi PD (inteso come server) e OF (come client). Sappiamo che l'applicativo principale lavora a 30 FPS e anche il server PD e' stato così sintonizzato. Si verificheranno sempre delle imperfezioni durante il funzionamento per cui determinati messaggi OSC che dovranno essere letti da OF frame per frame non verranno letti e si accumuleranno nella coda per essere esauriti il ciclo successivo.
Questo però crea il problema che, nel ciclo in cui il valore OSC in arrivo da PD non riesce ad essere letto in tempo, comunque occorre chiamare il metodo update del fiducial 'bass'. Ma in questo caso quale sarà il valore da passargli se non siamo riusciti ad aggiornarlo tramite PD? basterà fare in modo di inviare al fiducia l'ultimo valore memorizzato. Per cui, se prima la variabile 'livello_audio_basso' era locale al metodo 'update', ora la variabile diviene generale in modo da poter tenere in memoria il valore dell'ultima lettura da PD.

### NOTA 
ci sono due librerie che si occupano della manipolazione dei messaggi OSC in Pure data Extended.

La prima libreria e' quella a cura di JDL, facente parte del core di PureDataExtended (libreria "OSC") che conta gli oggetti [dumpOSC][OSCroute][sendOSC] .

La seconda libreria e' quella creata da Martin Peach e che fa uso degli oggetti [packOSC][unpackOSC][routeOSC][pipelist][packOSCstream][unpackOSCstream]. La libreria si fonda sull'uso di altri 2 oggetti creati dallo stesso Peach ( [udpreceive][udpsend] ). Gli oggetti sopra elencati funzionano SOLO se viene importata la libreria ([import mrpeach] ).
(dalla reference on-line - http://puredata.info/downloads/osc/#refrouteOSC - "a collection of Pd objectclasses for OSC-messages. these objects only convert between Pd-messages and OSC-messages (binary format), so you will need a separate set of objects that implement the transport (OSI-Layer 4), for instance [udpsend]/[udpreceive] for sending OSC over UDP.").


## videotavolo-v1.5.0

### SOLVED - issue 06
Aggiungere un controllo temporale che conta quanto tempo e' trascorso dall'apertura del programma e lo visualizza nelle schermate di setup o di debug.

### NOTE
iniziano i lavori sulla issue 05:
1) ci sono due librerie che si occupano della manipolazione dei messaggi OSC in Pure data Extended.
La prima libreria e' quella a cura di JDL, facente parte del core di PureDataExtended (libreria "OSC") che conta gli oggetti [dumpOSC][OSCroute][sendOSC] .
La seconda libreria e' quella creata da Martin Peach e che fa uso degli oggetti [packOSC][unpackOSC][routeOSC][pipelist][packOSCstream][unpackOSCstream]. La libreria si fonda sull'uso di altri 2 oggetti creati dallo stesso Peach ( [udpreceive][udpsend] ). 
Gli oggetti sopra elencati funzionano SOLO se viene importata la libreria ([import mrpeach] ).
(dalla reference on-line - http://puredata.info/downloads/osc/#refrouteOSC - "a collection of Pd objectclasses for OSC-messages. these objects only convert between Pd-messages and OSC-messages (binary format), so you will need a separate set of objects that implement the transport (OSI-Layer 4), for instance [udpsend]/[udpreceive] for sending OSC over UDP.").
1bis) abbiamo migliorato la patch di Pure Data uniformando la parte preposta alla'analisi dei messaggi OSC in arrivo da OF e in partenza verso OF. Abbiamo utilizzato solo oggetti della libreria core (JDL). Il 'toggle' OSC ora abilita o meno l'invio dei messaggio verso OF.
2) La patch di PD Extended e' stata migliorata provvedendo a ridurre la frequenza con cui i messaggi OSC (relativi al livello del basso) vengono inviati ad OF: se prima i messaggi veninvano sempre inviati (con frequenza di circa 90 valori al secondo), ora abbiamo ridotto la frequenza pareggiandola al FPS dell'applicazione.
3) Ci chiediamo se sia il caso di introdurre un controllo sul valore da inviare via OSC a OF. Se tale valore e' pari a 0 allora non ha senso inviarlo più di una volta e, se il valore rimane costante su 0, si potrebbe interrompere l'invio di messaggi OSC per riprenderlo poi non appena questo livello diventi nuovamente maggiore di 0.

## videotavolo-v1.4.4

### SOLVED - issue 04
Rendere tutte le animazioni dipendenti dal valore FPS in modo tale poter cambiare tale valore in un unico punto nel codice (o in punti limitati) affinche' tutte le animazioni lo rispettino. Modifiche apportate a tutte le classi fiducial. 


## videotavolo-v1.4.3

### SOLVED - issue 03
testApp riga 1691 (e non solo) e sulle patch di pure data modifica nomenclatura: da "transpose" a "transport".

## videotavolo-v1.4.2

### SOLVED - issue 02
Resta ancora da capire come possibile migliorare la rotazione affinche' la grafica dei fiducial resti associata ad essi anche nel ruotare (forse un problema causato dall'inclinazione del proiettore).

### NOTE
da alcuni esperimenti si e' potuto notare che la rotazione della grafica rispetto a quella del fiducial si discosta peridicamente di un valore varibile. 
Questa variazione dell'offset sula rotazione e' periodica con periodo PI/2. la spiegazione potrebbe essere legata al fatto di una cattiva calibrazione iniziale secondo la griglia.


## videotavolo-v1.4.1

### SOLVED - issue 01
Occorre ancora implementare un sistema che renda la variazione di BPM meno brusca sull'animazione del fiducial - provare a cambiare il BPM mentre sul tavolo e' disposto il fiducial Synth

### NOTE
si potrebbe rendere autonomo il fiducial synth nel fare il polling dei dati ad ogni croma.
il valore recuperato dall'esterno (letto dalla variabile statica bpm) viene confrontato con il valore memorizzato internamente all'oggetto.
Se sono identici, il calcolo del seno continua come se nulla fosse. Se invece il valore e' cambiato occorre capire come fare una animazione che ne tenga conto senza effetti di cambiamento bruschi

## videotavolo-v1.4

## videotavolo-v1.3
- migliorata la routine per l'eliminazione dei suoni residui (cancellazione e pulizia dei vettori)
- migliorato il debug (aggiunta di informazioni in merito al numero di fiducials presenti)
- liste sostituite con vettori
- rinominati alcuni degli oggetti e delel liste (sint_list -> synth_vec; pads -> synth; etc...)
- ora la dilatazione/compressione della grafica del PAD si muove in sincronia con il BPM
- ridotta l'espansione del fiducial Bass;
### TODO: 
- occorre ancora implementare un sistema che renda la variazione di BPM meno brusca sull'animazione del fiducial - provare a cambiare il BPM mentre sul tavolo e' disposto il fiducial Synth
- resta ancora da capire come possibile migliorare la rotazione affinche' la grafica dei fiducial resti associata ad essi anche nel ruotare (forse un problema causato dall'inclinazione del proiettore).

## videotavolo-v1.2
- migliorato la gestione del sonoro (uso alternato di LibPD e PD via OSC ora possibile e stabile)
- inserito lo sfondo con i loghi 
- implementato una routine per eliminare i suoni residui eventualmente ancora in riproduzione presso la patch di PD nel caso la si utilizzi via OSC e si chiuda l'appicativo OF prima di togliere i fiducia da sopra la superficie (succedeva nel caso dei Pads).
