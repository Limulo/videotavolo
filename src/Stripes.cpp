/*
 *  Stripes.cpp
 *  Prototipo3
 *
 *  Created by Limulo.
 *	http://www.limulo.net
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
	

