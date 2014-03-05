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
/*
 *  Fid_Synth.h
 *  Prototipo3
 *
 *  Created by Limulo.
 *  http://www.limulo.net 
 *
 *  Questa sottoclasse serve per dare una rappresentazione grafica al fiducial che rappresenta il PAD
 *
 */

#ifndef _INC_FID_SYNTH
#define _INC_FID_SYNTH

#include "Fid_Base.h"
#include "ofMain.h"
#include <math.h>
#include <stdio.h>

#define PAD_MAX_AMP		35	// ampiezza massima dell'oscillazione per il disegno del pad

#define STEP_IN			25	// velocità che impiega il fiducial a comparire sullo schermo
#define	STEP_OUT		12	// velocità che impiega il fiducial a scomparire dallo schermo


class Fid_Synth: virtual public Fid_Base {
private:
	int r;
	int g;
	int b;
	
	// due variabili di utilità per l'animazione
	int count;
	float r_osc;
	
public:
	Fid_Synth(int _fid, int _sid);
	~Fid_Synth() {std::cout << "FID SYNTH: De-constructiong derived: Fid_Synth!\n" << std::endl; }
	
	void setup(ofVec2f *_fid_pos, ofVec2f *_ctr_pos, float _fid_angle, ofColor _color);
	void added();
	
	void update_interrupt(ofVec2f *fiducial_pos, ofVec2f *centro, float angolo_attuale, float vel);
	void update_continuos(int a);
	//void update_continuos();
	
	void removed();
	
	void draw();
	
	ofVec2f* getFidPos();
	bool isAlive();	
	
	void inside(ofVec2f *p);
	void debug();

};

#endif
