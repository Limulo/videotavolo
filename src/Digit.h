// INIT_PAUSA : fino alla versione 1.4.3 si trattava del numero di frame per cui il digit rimane in vista.
// il valore originale è pari a 120 ossia, dato 60 il valore di FPS nel programma originale, pari
// a 2 secondi.

#ifndef _DIGIT_INIT
#define _DIGIT_INIT
#define DIGIT_FPS 60

#include "ofMain.h"

#define INIT_PAUSA 2*DIGIT_FPS


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
	Digit() { std::cout << "DIGIT costruito!" << std::endl; };
	~Digit() { std::cout << "DIGIT distrutto!" << std::endl; };
	
	void setup(void);
	void update(void);
	void draw(int lpos_);
	void set_bpm(int bpm_);
	
	
};

#endif