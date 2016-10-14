// Questa sottoclasse serve per dare una rappresentazione grafica al fiducial dedicato alla modifica del tempo BPM


#ifndef _INC_FID_ROT
#define _INC_FID_ROT

#include "ofMain.h"
#include <math.h>
#include <stdio.h>
#include <iostream>
#include "Fid_Base.h"

#define CORONA_L		20		// larghezza della corona circolare
#define CORONA_A		120		// angolo totale di apertura della corona circolare (espresso in gradi)
#define CORONA_RES		35		// risoluzione degli archi che disegnano la corona
#define CORONA_OFF		10		// distanza tra la corona e il bordo del fiducial

#define FIDUCIAL_MER	PI/2.0	// Maximum Emi Rotation (massima semirotazione del fiducial) (espressa in radianti)
								// in pratica stabilisce quanto devo ruotare il fiducial sul tavolo
								// per riempire la corona circolare.
								// coinvolge l'angolo 'fid_limited_angle'


class Fid_Rot: virtual public Fid_Base 
{
private:
	
protected:
	float	c_innerRadius;		// raggio del fiducial (raggio del tondo pieno)
	float	c_outerRadius;		// raggio della corona circolare (bordo più interno)
	float	c_fill_angle;		// angolo della corona circolare di riempimento
	
	ofColor	c_color;			// 	
	int fr, fg, fb;				// fiducial color (colore del tondo pieno)
	int cr, cg, cb;				// corona color (colore della corona circolare)
	
	ofPath	c_fill;				// corona circolare: a riempimento
	ofPath	c_bordo;			// corona circolare: bordo esterno
	ofVec3f	z_axis;				// asse-z
	
	float	fid_limited_angle;	// conserva il dato di rotazione passato da TUIO, limitato tra i valori (-FIDUCIAL_MER, FIDUCIAL_MER)

	
public:
	Fid_Rot(int _fid, int _sid);
	~Fid_Rot() { /*cout << "FID ROT: De-constructiong derived: Fid_Rot" << endl;*/ }
	
	void setup(ofVec2f *_fid_pos, ofVec2f *_ctr_pos, float _fid_angle, float _lim_angle, ofColor _fColor, ofColor _cColor);
	
	void added();
	void update_interrupt(ofVec2f *_fid_pos, ofVec2f *_ctr_pos, float _fid_angle, float _fid_rot_vel);
	void update_continuos(int playHeadPos_);
	void removed();

	void draw();
	
	ofVec2f* getFidPos();
	bool isAlive();	
	float get_lim_angle();
	
	void inside(ofVec2f *p);
	void debug();
	 
};

#endif
