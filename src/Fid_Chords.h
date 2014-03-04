/*
 *  Fid_Chords.h
 *  Prototipo3
 *
 *  Created by Limulo.
 *  http://www.limulo.net 
 *  
 *  Questa sottoclasse serve per dare una rappresentazione grafica ai fiducial che rappresentano gli accordi
 *  Per molti aspetti, questa sottoclasse è simile alla Fid_Rot.h
 *
 */

#ifndef _INC_FID_CHORDS
#define _INC_FID_CHORDS

#include "ofMain.h"
#include <math.h>
#include <stdio.h>
#include <iostream>
#include "Fid_Base.h"

#define CORONA_L			20		// larghezza della corona circolare
#define CORONA_CHORDS_A		320		// angolo di apertura della corona circolare (GRADI)
#define CORONA_RES			35		// risoluzione degli archi che disegnano la corona
#define CORONA_OFF_CHORDS	20		// distanza tra la corona e il bordo del fiducial

#define STEP_IN			25		// velocità che impiega il fiducial a comparire sullo schermo
#define	STEP_OUT		12		// velocità che impiega il fiducial a scomparire dallo schermo


class Fid_Chords: virtual public Fid_Base 
{
private:
	
protected:
	float	c_innerRadius;
	float	c_outerRadius;
	
	ofColor	c_color;
	ofPath	c_fill;				// corona circolare: a riempimento
	ofPath	c_bordo;			// corona circolare: bordo esterno
	ofVec3f	z_axis;				// asse-z
	
	float	fid_limited_angle;	// conserva il dato di rotazione passato da TUIO, limitato tra i valori (-FIDUCIAL_MER, FIDUCIAL_MER)
	
	int fr, fg, fb;				// fiducial color (colore del tondo pieno)
	int cr, cg, cb;				// corona color (colore della corona circolare)
	
public:
	Fid_Chords(int _fid, int _sid);
	~Fid_Chords() {cout << "FID CHORDS: De-constructiong derived: Fid_Chords\n"; }
	
	void setup(ofVec2f *_fid_pos, ofVec2f *_ctr_pos, float _fid_angle, ofColor _cColor);
	
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

