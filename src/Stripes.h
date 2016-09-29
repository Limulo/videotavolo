//	Questa classe ha il compito di identificare e gestire una porzione verticale di schermo
//  Lavorando in simbiosi con la playhead, restituisci l'impressione che questa, muovendosi, lasci una scia colorata
// fadeStep : è il valore che viene sottratto al valore alfa di ogni striscia frame per frame. Ad esempio, se fadeStep 
//			vale 20, significa che, per un FPS pari a 60 come da programma originale, il tempo che la striscia impiega 
//			a dissolversi è di circa 212ms.


#ifndef _INC_STRIPES
#define _INC_STRIPES

#define STRIPE_FADE_STEP 20

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
	Stripe() { std::cout << "STRIPE: costruito!" << std::endl; };
	~Stripe() { std::cout << "STRIPE: distrutto!" << std::endl; };
	
	void init(int lx_, int ty_, int w_, int h_, ofColor _cStripe);

	void checka(int phx_);

};


#endif

