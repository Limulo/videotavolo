// Questa sottoclasse serve per dare una rappresentazione grafica ai fiducial cui è stata assegnata una forma rotonda
// si tratta del fiducial KICK

#ifndef _INC_FID_ROUND
#define _INC_FID_ROUND

#include "ofMain.h"
#include <math.h>
#include <stdio.h>
#include <iostream>
#include "Fid_Base.h"

#define CIRCLE_RES		35		// risoluzione delcerchio


class Fid_Round: virtual public Fid_Base 
{
private:
	int r, g, b;				// fiducial color (colore del tondo pieno)
  
protected:
	float	c_innerRadius;
	
	ofColor	c_color;
	ofVec3f	z_axis;				// asse-z
	
public:
	Fid_Round(int _fid, int _sid);
	~Fid_Round() { /*cout << "FID ROUND: De-constructiong derived: Fid_Round" << endl;*/ }
	
	void setup(ofVec2f *_fid_pos, ofVec2f *_ctr_pos, float _fid_angle, ofColor _fColor);
	
	void added();
	void update_interrupt(ofVec2f *_fid_pos, ofVec2f *_ctr_pos, float _fid_angle, float _fid_rot_vel);
	void update_continuos(int playHeadPos_); 
	void removed();
	
	void draw();
	
	ofVec2f* getFidPos();
	bool isAlive();	
	
	void inside(ofVec2f *p);
	void debug();
	
};

#endif
