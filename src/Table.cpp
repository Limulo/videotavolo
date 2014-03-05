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

#include "ofMain.h"
#include "Table.h"
#include <iostream>

using namespace std;


// COSTRUTTORE /////////////////////////////////////////////////////////
Table::Table()
{
	// init();
}

// DISTRUTTORE /////////////////////////////////////////////////////////
Table::~Table()
{
	cout << "TABLE: Tabella distrutta.\n";
}



// INIZIALIZZAZIONE ////////////////////////////////////////////////////
void Table::init()
{
	ofLog() << "\tTABLE INIT: inizializzazione della tabella" << endl;
	int riga, colonna, piano;
	
	for (colonna=0; colonna<COLS; colonna++)
	{
		for (riga=0; riga<ROWS; riga++)
		{
			for (piano=0; piano<PLANES; piano++)
			{
				fiducial_table[riga][colonna][piano]=0;
			}
		}
	}	
}


// ADD TO TABLE ////////////////////////////////////////////////////////
void Table::add_to_table(int colonna, int piano, int sess_id)
{
	int riga;
	for (riga=0; riga<ROWS; riga++)
	{
		if (fiducial_table[riga][colonna][0]==0)
		{
			fiducial_table[riga][colonna][0]=sess_id;
			break;
		}
	}
	for (int p=1; p<PLANES; ++p)
	{
		if (p==piano)
			fiducial_table[riga][colonna][p]=1;
		else
			fiducial_table[riga][colonna][p]=0;
	}
	//print_table(0);
}


// UPDATE TABLE ////////////////////////////////////////////////////////
void Table::update_table(int colonna, int piano, int sess_id)
{
	remove_from_table(piano, sess_id);
	add_to_table(colonna, piano, sess_id);
}


// REMOVE FROM TABLE ///////////////////////////////////////////////////
void Table::remove_from_table(int piano, int sess_id)
{
	int riga, colonna;
	for (colonna=0; colonna<COLS; colonna++)
	{
		for (riga=0;riga<ROWS;riga++)
		{
			if (fiducial_table[riga][colonna][0]==sess_id)
			{
				fiducial_table[riga][colonna][0]=0;
				fiducial_table[riga][colonna][piano]=0;
			}
		}
	}
	//print_table(0);
}


// PRINT TABLE /////////////////////////////////////////////////////////
void Table::print_table(int piano)
{
	int riga, colonna;
	for(riga=0; riga<ROWS; riga++)
	{
		for (colonna=0; colonna<COLS; colonna++)
		{
			cout << fiducial_table[riga][colonna][piano] << " ";
		}
		cout << "\n";
	}
}
	
	
	

		

