#include "Fid_Base.h"


// COSTRUTTORE /////////////////////////////////////////////////////////
Fid_Base::Fid_Base() {
	//cout << "FID_BASE: Constructiong!" << endl;;
	
	// di seguito inzializziamo tutti gli elementi che accomununano tutti i vari fiducials tra di loro
	tExpand   = ( 10 * FIDUCIAL_FPS) / 60; // valore temporale (espresso in frames) per l'espansione
	tCollapse = (480 * FIDUCIAL_FPS) / 60; // valore temporale (espresso in frames) per il ritorno alle normali dimensioni
	
	fid_pos.set(0.0f, 0.0f);
	
	transparency = 0;
	stato = STABLE;
	bAlive = false;
	
	// variabili utili per l'animazione del trigger
	// ossia quando la playhead raggiunge la posizione del fiducial
	bTrigger = false;
	bExpand = false;
	bCollapse = false;
	
	aTrigger = 80;		// ampiezza di dilatazione della forma del fiducial quando venga triggerato
	startFrame = 0;
	marginTrigger = 30;	// raggio di un cerchio immaginario, tracciato centrendolo sulla posizione del fiducial.	
}



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
