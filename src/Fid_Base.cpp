/*
 *  Fid_Base.cpp
 *  Prototipo3
 *
 *  Created by Limulo.
 *  http://www.limulo.net 
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
