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

//	Questa classe ha il compito di identificare e gestire una porzione verticale di schermo
//  Lavorando in simbiosi con la playhead, restituisci l'impressione che questa, muovendosi, lasci una scia colorata


#ifndef _INC_STRIPES
#define _INC_STRIPES

#include "ofMain.h"

class Stripe 
{
	
private:
	int lx, rx;		// left & right x
	int ty, by;		// top & bottom y
	int w, h;
	int alpha;
	int fadeStep;
	int r, g, b;
	
public:
	Stripe(){};
	~Stripe() {};
	
	void init(int lx_, int ty_, int w_, int h_, int fadeStep_, ofColor _cStripe);
	
	void update(int fadeStep_);
	void checka(int phx_);

};


#endif

