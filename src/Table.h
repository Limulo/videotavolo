/*
 * videoTavolo
 * ===========
 *
 * INFO
 * ===========
 * videoTavolo è un prototipo di superficie interattiva. 
 * Vengono qui rilasciati i codici sorgente e le patches di PureData del progetto. 
 * Il progetto è curato da Limulo ( http://www.limulo.net ) con i seguenti contributi esterni:
 *
 * 1) ofxPd
 * Copyright (c) Dan Wilcox 2011-2013
 * BSD Simplified License.
 * https://github.com/danomatika/ofxPd
 * 
 * 2) ofxTuio
 * permette di creare e gestire direttamente nell'ambiente 
 * di sviluppo di openFrameworks un server e un client che 
 * comunicano tramite protocollo TUIO
 * https://github.com/patriciogonzalezvivo/ofxTuio
 * 
 * LICENZA
 * ===========
 * ad eccezione degli elementi elencati qui sopra, tutto il codice è rilasciato da Limulo secondo la licenza 
 * Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0). Per prendere visione di una copia 
 * di tale licenza visitate http://creativecommons.org/licenses/by-sa/4.0/ .
 * 
 */
 
//	Classe per la creazione della matrice di organizzazione e riproduzione dei suoni



#ifndef _INC_TABLE
#define _INC_TABLE

/* 
 *  La matrice che qui andremo a descrivere è una matrice tridimensionale
 *  composta da:
 *  8 COLONNE
 *  3 RIGHE
 *  4 PIANI
 *  il primo piano è dedicato a contenere i session ID
 *  gli altri 3 piani rimanenti (1 per CASSA, 2 per SNARE, e 3 per HIHAT) contengono UNI o ZERI 
 */
 

#define ROWS 3
#define COLS 8
#define PLANES 4


class Table
{
	int fiducial_table[ROWS][COLS][PLANES];
public:
	Table();
	~Table();
	void init();
	void add_to_table(int colonna, int piano, int sess_id);
	void update_table(int colonna, int piano, int sess_id);
	void remove_from_table(int piano, int sess_id);
	void print_table(int piano);
	int	 get_element(int r, int c, int p){return fiducial_table[r][c][p];}
};

#endif
