#include "Stripes.h"

void Stripe::init(int lx_, int ty_, int w_, int h_, ofColor _cStripe) {
	lx = lx_;
	w = w_;
	rx = lx + w;
	
	ty = ty_;
	h = h_;
	by = ty + h;
	
	fadeStep = STRIPE_FADE_STEP;
	if(fadeStep < 0) {
		fadeStep = 1;
	}
	
	r = _cStripe.r;
	g = _cStripe.g;
	b = _cStripe.b;
	
	alpha = 0;
	
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
		ofDrawRectangle(lx, ty, wp, h);
	} 
	else if (alpha > 0) 
	{
		alpha -= fadeStep;
		
		if (alpha < 0 )
			alpha = 0;
		
		ofSetColor(r, g, b, alpha);
		ofFill();
		ofDrawRectangle(lx, ty, w, h);
		
	}
		
	ofDisableAlphaBlending();
	ofPopStyle();
}
	

