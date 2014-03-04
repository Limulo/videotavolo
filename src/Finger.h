/*
 *  finger.h
 *  Prototipo3
 *
 *  Created by Limulo.
 *  http://www.limulo.net 
 *  
 *  Questa classe serve per dare una rappresentazione grafica alle dita
 *
 */

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
	~Finger() {std::cout << "De-constructing Finger!\n"<<std::endl; };
	
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
