/*
 *  Stripes.h
 *  Prototipo3
 *
 *  Created by Limulo.
 *	http://www.limulo.net
 *
 *	Questa classe ha il compito di identificare e gestire una porzione verticale di schermo
 *  Lavorando in simbiosi con la playhead, restituisci l'impressione che questa, muovendosi, lasci una scia colorata
 *  
 */

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

