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
  
//  Questa classe serve per dare una rappresentazione grafica alle dita


#ifndef _INC_FINGER
#define _INC_FINGER
#include "ofMain.h"

#define FINGER_R	30	// raggio del finger (nella resa grafica)

class Finger {
private:
	int r;
	int g;
	int b;
	
public:
	int fid;			// finger ID
	
	ofVec2f	fing_pos;	// conserva i dati x e y passati da TUIO (float32, 0-1)
	ofColor f_color;
	
	enum State 
	{
		FADE_IN = 0,
		FADE_OUT,
		STABLE
	} stato;
	
	int	transparency;
		
	Finger(int _fid);
	~Finger() { std::cout << "De-constructing Finger!"<<std::endl; };
	
	void setup(ofVec2f *_fid_pos, ofColor _color);
	
	void added();
	
	void update_interrupt(ofVec2f *fid_pos);
	void update_continuos();
	
	void removed();

	void draw();
	
	ofVec2f* getFidPos();
	int get_finger_id();
	void debug();
	
	
	
};

#endif
