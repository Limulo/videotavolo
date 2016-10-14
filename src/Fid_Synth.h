#ifndef _INC_FID_ROT
#include "Fid_Rot.h"
#endif


#ifndef _INC_FID_SYNTH
#define _INC_FID_SYNTH

#include "Fid_Base.h"
#include "ofMain.h"
#include <math.h>
#include <stdio.h>

#define PAD_MAX_AMP		35	// ampiezza massima dell'oscillazione per il disegno del pad


class Fid_Synth: virtual public Fid_Base {
private:
	int r;
	int g;
	int b;
	
	static unsigned int synth_bpm;
	
	// variabili di utilità per l'animazione
	int numero_croma;
	float r_osc;
	float f;
	float t, t0;
	float T; // periodo dell'oscillazione
	
public:
	Fid_Synth(int _fid, int _sid);
	~Fid_Synth() { /*std::cout << "FID SYNTH: De-constructiong derived: Fid_Synth!" << std::endl;*/ }
	
	void setup(ofVec2f *_fid_pos, ofVec2f *_ctr_pos, float _fid_angle, ofColor _color);
	void added();
	
	void update_interrupt(ofVec2f *fiducial_pos, ofVec2f *centro, float angolo_attuale, float vel);
	void update_continuos(int playHeadPos_) { /* Do nothing */ };
	void update_continuos(float time_);
	void reset_internal_timer(int n_crome_);
	
	void removed();
	
	void draw();
	
	ofVec2f* getFidPos();
	bool isAlive();	
	
	void inside(ofVec2f *p);
	void debug();
	
	friend class ofApp; // so to make it possible ti ofApp to access private varaible 'synth_bpm'
	//friend class Fid_Rot;
};

#endif
