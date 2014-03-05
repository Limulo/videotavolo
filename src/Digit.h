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

#ifndef _DIGIT_INIT
#define _DIGIT_INIT


#include "ofMain.h"

#define INIT_PAUSA 120


class Digit {
	
private:
	ofTrueTypeFont  digitFont;
	int digit_pausa;
	int digit_alpha;
	int digit_alpha_step_in;
	int digit_alpha_step_out;
	int digit_tAppear;
	int digit_tDisappear;
	int digit_string_width;
	int digit_char_width;
	int digit_letter_space;
	int bpmValue;
	
	
public:
	Digit() {};
	~Digit() {};
	
	void setup(void);
	void update(void);
	void draw(int lpos_);
	void set_bpm(int bpm_);
};

#endif