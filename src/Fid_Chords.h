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
 
//  Questa sottoclasse serve per dare una rappresentazione grafica ai fiducial che rappresentano gli accordi
//  Per molti aspetti, questa sottoclasse è simile alla Fid_Rot.h


#ifndef _INC_FID_CHORDS
#define _INC_FID_CHORDS

#include "ofMain.h"
#include <math.h>
#include <stdio.h>
#include <iostream>
#include "Fid_Base.h"

#define CORONA_L			20		// larghezza della corona circolare
#define CORONA_CHORDS_A		320		// angolo di apertura della corona circolare (GRADI)
#define CORONA_RES			35		// risoluzione degli archi che disegnano la corona
#define CORONA_OFF_CHORDS	20		// distanza tra la corona e il bordo del fiducial


class Fid_Chords: virtual public Fid_Base 
{
private:
	
protected:
	float	c_innerRadius;
	float	c_outerRadius;
	
	ofColor	c_color;
	ofPath	c_fill;				// corona circolare: a riempimento
	ofPath	c_bordo;			// corona circolare: bordo esterno
	ofVec3f	z_axis;				// asse-z
	
	float	fid_limited_angle;	// conserva il dato di rotazione passato da TUIO, limitato tra i valori (-FIDUCIAL_MER, FIDUCIAL_MER)
	
	int fr, fg, fb;				// fiducial color (colore del tondo pieno)
	int cr, cg, cb;				// corona color (colore della corona circolare)
	
public:
	Fid_Chords(int _fid, int _sid);
	~Fid_Chords() { /*cout << "FID CHORDS: De-constructiong derived: Fid_Chords" << endl;*/ }
	
	void setup(ofVec2f *_fid_pos, ofVec2f *_ctr_pos, float _fid_angle, ofColor _cColor);
	
	void added();
	void update_interrupt(ofVec2f *_fid_pos, ofVec2f *_ctr_pos, float _fid_angle, float _fid_rot_vel);
	void update_continuos(int playHeadPos_);
	void removed();
	
	void draw();
	
	ofVec2f* getFidPos();
	bool isAlive();	
	float get_lim_angle();
	
	void inside(ofVec2f *p);
	void debug();
	
};

#endif

