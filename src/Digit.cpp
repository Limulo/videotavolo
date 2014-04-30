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

// digit_tDisappear : si tratta del tempo che il digit impiega a scomparire dallo schermo. Fino alla versione
//			1.4.3 era espresso in frames ed era pari a 60 (dato 60 il valore di FPS come da programma originale)
//			pari a 1 secondo.

#include "Digit.h"

void Digit::setup() {
	
	//ofLog() << "\tDIGIT SETUP: setup" << endl;
	cout << "\tDIGIT SETUP: setup" << endl;
	
	ofTrueTypeFont::setGlobalDpi(72);
	digitFont.loadFont("DS-DIGII.ttf", 80, true, true);
	digitFont.setLineHeight(18.0f);
	digitFont.setLetterSpacing(1.037);
	
	digit_pausa =		INIT_PAUSA;
	digit_alpha =		0;

	//digit_tAppear =		60;
	digit_tDisappear =	1*DIGIT_FPS;
	
	//digit_alpha_step_in  = 255 / digit_tAppear;
	digit_alpha_step_out = 255 / digit_tDisappear;
	
	bpmValue = 0;
	
	digit_string_width	= digitFont.stringWidth("000");
	digit_char_width	= digitFont.stringWidth("0");
	digit_letter_space	= (digit_string_width -  (digit_char_width*3) ) / 2;
	
	//std::cout << digit_string_width << " - " << digit_char_width << std::endl;
}

void Digit::update() {
	
	if(digit_pausa > 0) {
		digit_pausa --;
	} else if (digit_alpha > 0) {
		digit_alpha -= digit_alpha_step_out;
	} else {
		digit_alpha = 0;
	}

}

void Digit::draw(int lpos_) {
	
	//font che mostra il valore del BPM
	int unita =		 bpmValue %10;
	int decine =	(bpmValue %100)  /10;
	int centinaia = (bpmValue %1000) /100;
	
	ofPushStyle();
	ofSetColor(0, 255, 0, (int)digit_alpha/4);
	
	ofPushMatrix();
	ofTranslate(lpos_ - digit_string_width, 100, 0);
	digitFont.drawString("0", 0, 0);
	digitFont.drawString("0", digit_char_width + digit_letter_space, 0);
	digitFont.drawString("0", digit_char_width + digit_letter_space + digit_char_width, 0);
	
	ofSetColor(0, 255, 0, digit_alpha);
	if(centinaia == 1)		digitFont.drawString(ofToString(centinaia), 24, 0);
	else if(centinaia != 0)	digitFont.drawString(ofToString(centinaia), 0, 0);
	
	if(decine == 1)	digitFont.drawString(ofToString(decine), digit_char_width + digit_letter_space + 24, 0);
	else			digitFont.drawString(ofToString(decine), digit_char_width + digit_letter_space, 0);
	
	if(unita == 1)	digitFont.drawString(ofToString(unita), digit_char_width + digit_letter_space + digit_char_width + 24, 0);
	else			digitFont.drawString(ofToString(unita), digit_char_width + digit_letter_space + digit_char_width, 0);
	
	ofPopMatrix();				
	ofPopStyle();
	
	
}

void Digit::set_bpm(int bpm_) {
	bpmValue = bpm_;
	digit_pausa = INIT_PAUSA;
	digit_alpha = 255;
	
	
}