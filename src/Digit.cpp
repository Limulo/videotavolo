/*
 *  Digit.cpp
 *  Prototipo3_v6
 *
 *  Created by Nicola Ariutti on 19/10/13.
 *  Copyright 2013 Limulo. All rights reserved.
 *
 */

#include "Digit.h"

void Digit::setup() {
	
	ofLog() << "\tDIGIT SETUP: setup" << endl;
	
	ofTrueTypeFont::setGlobalDpi(72);
	digitFont.loadFont("DS-DIGII.ttf", 80, true, true);
	digitFont.setLineHeight(18.0f);
	digitFont.setLetterSpacing(1.037);
	
	digit_pausa =		INIT_PAUSA;
	digit_alpha =		0;

	//digit_tAppear =		60;
	digit_tDisappear =	60;
	
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