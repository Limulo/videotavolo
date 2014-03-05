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

#include "Stripes.h"


void Stripe::init(int lx_, int ty_, int w_, int h_, int fadeStep_, ofColor _cStripe) {
	lx = lx_;
	w = w_;
	rx = lx + w;
	
	ty = ty_;
	h = h_;
	by = ty + h;
	
	fadeStep = fadeStep_;
	
	r = _cStripe.r;
	g = _cStripe.g;
	b = _cStripe.b;
	
}

void Stripe::update(int fadeStep_) 
{
	fadeStep = fadeStep_;
	if(fadeStep < 0) 
	{
		fadeStep = 1;
	}
}

void Stripe::checka(int phx_) 
{

	ofPushStyle();
	ofEnableAlphaBlending();
	
	// effettuiamo un controllo sulla poszione della playhead 
	// rispetto alle coordinate della striscia verticale
	if(phx_ > lx && phx_ < rx) 
	{
		alpha = 180;
		
		ofSetColor(r, g, b, alpha);
		ofFill();
		int wp = phx_ - lx;
		ofRect(lx, ty, wp, h);
	} 
	else if (alpha > 0) 
	{
		alpha -= fadeStep;
		
		if (alpha < 0 )
			alpha = 0;
		
		ofSetColor(r, g, b, alpha);
		ofFill();
		ofRect(lx, ty, w, h);
		
	}
		
	ofDisableAlphaBlending();
	ofPopStyle();
}
	

