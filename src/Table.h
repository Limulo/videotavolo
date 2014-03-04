/*
 *  Table.h
 *  Prototipo3
 *
 *  Created by Limulo.
 *  http://www.limulo.net 
 *  
 *	Classe per la creazione della matrice di organizzazione e riproduzione dei suoni
 */


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
