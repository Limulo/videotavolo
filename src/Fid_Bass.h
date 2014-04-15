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
 
// Questa sottoclasse serve per dare una rappresentazione grafica al fiducial che rappresenta il BASS

#ifndef _INC_FID_BASS
#define _INC_FID_BASS

#include "ofMain.h"
#include <math.h>
#include <stdio.h>
#include <iostream>
#include "Fid_Base.h"

#define CIRCLE_RES		35		// risoluzione delcerchio

#define BASS_MAX_AMP	6		// quando suona il basso questa è la massima area del cerchio


//#define STEP_IN			25		// velocità che impiega il fiducial a comparire sullo schermo
//#define	STEP_OUT		12		// velocità che impiega il fiducial a scomparire dallo schermo
//#define STEP_IN			int( 255.0f / ( FIDUCIAL_FPS * (170.0f / 1000.0f) ) )
//#define	STEP_OUT		int( 255.0f / ( FIDUCIAL_FPS * (354.0f / 1000.0f) ) )


class Fid_Bass: virtual public Fid_Base 
{
private:
	
protected:
	float	c_innerRadius;
	float	bass_level;
	ofColor	c_color;
	ofVec3f	z_axis;				// asse-z
		
	int fr, fg, fb;				// fiducial color (colore del tondo pieno)
	
public:
	Fid_Bass(int _fid, int _sid);
	~Fid_Bass() {cout << "FID BASS: De-constructiong derived: Fid_Bass\n";}
	
	void setup(ofVec2f *_fid_pos, ofVec2f *_ctr_pos, float _fid_angle, ofColor _fColor);
	
	void added();
	void update_interrupt(ofVec2f *_fid_pos, ofVec2f *_ctr_pos, float _fid_angle, float _fid_rot_vel);
	void update_continuos(int bass_level_); 
	void removed();
	
	void draw();
	
	ofVec2f* getFidPos();
	bool isAlive();	
	
	void inside(ofVec2f *p);
	void debug();
	
	
};

#endif
