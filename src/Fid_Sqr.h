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

// Questa sottoclasse serve per dare una rappresentazione grafica ai fiducial cui è stata assegnata una forma quadrata
// si tratta dei fiducial SNARE e HIHAT


#ifndef _INC_FID_SQR
#define _INC_FID_SQR

//#define FIDUCIAL_FPS	60


#include "Fid_Base.h"
#include "ofMain.h"
#include <math.h>
#include <stdio.h>

//#define STEP_IN			int( 255 / ( FIDUCIAL_FPS * (170 / 1000) ) )
								// velocità che impiega il fiducial a comparire sullo schermo. 
								// Con un valore di 25, un FPS impostato su 60 nel programma originale, 
								// l'animazione 'comparsa' impiega 10,2 frames per svolgersi, per un tempo
								// pari a 170ms.

//#define	STEP_OUT		int( 255 / ( FIDUCIAL_FPS * (354 / 1000) ) )
								// velocità che impiega il fiducial a scomparire dallo schermo
								// Con un valore di 12, un FPS impostato su 60 nel programma originale, 
								// l'animazione 'scomparsa' impiega 21,25 frames per svolgersi, per un tempo
								// pari a 354,16ms.


class Fid_Sqr: virtual public Fid_Base {
private:
	int r;
	int g;
	int b;
	
	
public:
	Fid_Sqr(int _fid, int _sid);
	~Fid_Sqr() {std::cout << "FID SQR: De-constructiong derived: Fid_Sqr!\n" << std::endl; }
	
	void setup(ofVec2f *_fid_pos, ofVec2f *_ctr_pos, float _fid_angle, ofColor _color);
	void added();
	
	void update_interrupt(ofVec2f *fiducial_pos, ofVec2f *centro, float angolo_attuale, float vel);
	void update_continuos(int playHeadPos_);
	
	void removed();
	
	void draw();
	
	ofVec2f* getFidPos();
	bool isAlive();	
	
	void inside(ofVec2f *p);
	void debug();
	
};

#endif


