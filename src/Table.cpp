#include "ofMain.h"
#include "Table.h"
#include <iostream>

using namespace std;


// COSTRUTTORE /////////////////////////////////////////////////////////
Table::Table()
{
	// init();
	cout << "TABLE: Tabella costruita!" << endl;
}

// DISTRUTTORE /////////////////////////////////////////////////////////
Table::~Table()
{
	cout << "TABLE: Tabella distrutta." << endl;
}



// INIZIALIZZAZIONE ////////////////////////////////////////////////////
void Table::init()
{
	//ofLog() << "\tTABLE INIT: inizializzazione della tabella" << endl;
	cout << "\tTABLE INIT: inizializzazione della tabella" << endl;
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
	
	
	

		

