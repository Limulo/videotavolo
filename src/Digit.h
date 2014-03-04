/*
 *  Digit.h
 *  Prototipo3_v6
 *
 *  Created by Nicola Ariutti on 19/10/13.
 *  Copyright 2013 Limulo. All rights reserved.
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