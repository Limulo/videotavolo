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

#include "Fid_Base.h"


// DISTANZA ////////////////////////////////////////////////////////////
float Fid_Base::distanza(ofVec2f *a, ofVec2f *b) 
{
	float d1 = abs(a->x - b->x);
	float d2 = abs(a->y - b->y);
	float d3 = ( (d1*d1) + (d2*d2) );
	
	// occorre fare un controllo per assicurarsi che 'distanza' 
	// non sia nulla. Il problema è che 'distanza' appare a 
	// denominatore nella formula per il calcolo dell'arco-coseno
	if (d3 == 0) 
	{
		return 0.001f;
	} 
	else
	{
		return d3;
	}
	
}


// GET FIDUCIAL ID /////////////////////////////////////////////////////
int Fid_Base::get_f_id() 
{
	return fid;
}



// GET SESSION ID //////////////////////////////////////////////////////
int Fid_Base::get_s_id() 
{
	return sid;
}
