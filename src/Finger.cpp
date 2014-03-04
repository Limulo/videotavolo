/*
 *  finger.cpp
 *  Prototipo3
 *
 *  Created by Limulo.
 *  http://www.limulo.net 
 *
 */

#include "Finger.h"

#define STEP_IN			25		// velocità che impiega il fiducial a comparire sullo schermo
#define	STEP_OUT		12		// velocità che impiega il fiducial a scomparire dallo schermo


// COSTRUTTORE /////////////////////////////////////////////////
Finger::Finger(int _fid)
{
	std::cout << "Constructing Finger!\n" << std::endl;
	fid = _fid;
	
	//fid_pos
	fing_pos.set(0.0f, 0.0f);
	transparency = 0;
	stato = STABLE;
}


// SETUP ///////////////////////////////////////////////////////
void Finger::setup(ofVec2f *_fid_pos, ofColor _color)
{
	// UPDATE POSITION -----------------------------------------
	fing_pos.set(_fid_pos->x, _fid_pos->y);
	
	r = _color.r;
	g = _color.g;
	b = _color.b;
}


// ADDED ///////////////////////////////////////////////////////
void Finger::added()
{
	stato = FADE_IN;
}

// UPDATES /////////////////////////////////////////////////////
void Finger::update_interrupt(ofVec2f *_fid_pos)
{
	// UPDATE POSITION --------------------------------------------------------------
	fing_pos.set(_fid_pos->x, _fid_pos->y);
}


void Finger::update_continuos()
{
	// STATE UPDATE -----------------------------------------------
	if(stato != STABLE) 
	{
		if (stato == FADE_IN) 
		{
			transparency += STEP_IN;
			if(transparency > 255 )
			{ 
				transparency = 255;
				stato = STABLE;
			} 
		} 
		else if (stato == FADE_OUT) 
		{
			transparency -= STEP_OUT;
			if (transparency < 0)
			{
				stato = STABLE;
				transparency = 0;
				//alive = false;
			} 
		}
	}
	
	f_color.set(r, g, b, transparency);
	
}


// REMOVED /////////////////////////////////////////////////////
void Finger::removed(void)
{
	stato = FADE_OUT;
}


// DRAW ////////////////////////////////////////////////////////
void Finger::draw(void)
{
	ofEnableAlphaBlending();
	ofPushMatrix();
	ofPushStyle();
	ofTranslate(fing_pos);
	
	ofFill();
	ofCircle(0, 0, FINGER_R, FINGER_R);
	
	ofPopStyle();
	ofPopMatrix();
	ofDisableAlphaBlending();	
}




// GETTERS /////////////////////////////////////////////////////
ofVec2f* Finger::getFidPos() 
{
	return &fing_pos;
}

int Finger::get_finger_id() 
{
	return fid;
}


// OTHER ///////////////////////////////////////////////////////
void Finger::debug() 
{
	// TESTO a schermo ------------------------------------------
	ofPushMatrix();
		ofPushStyle();
		ofTranslate(fing_pos);
	
		ofSetHexColor(0x000000);
	
		// RIQUADRO 1 -----------------------------------------------
		ofPushMatrix(); 
			ofTranslate(-50, -100 , 0);
			ofDrawBitmapString("Finger_Pos X: " + ofToString(fing_pos.x) + "\nFinger_Pos Y: " + ofToString(fing_pos.y), 0, 0);
		ofPopMatrix();
	
		// RIQUADRO 4 -----------------------------------------------
		ofPushMatrix();
			ofTranslate(-50, - 100 , 52);
		
			ofDrawBitmapString("Finger ID: " + ofToString((int)fid), 0, 0);
	
	
			switch (stato)
			{
				case STABLE:
					ofDrawBitmapString("state: STABLE\n", 0, 26);
					break;
				case FADE_IN:
					ofDrawBitmapString("state: FADE IN\n", 0, 26);
					break;
				case FADE_OUT:
					ofDrawBitmapString("state: FADE OUT\n", 0, 26);
					break;
				default:
					ofDrawBitmapString("NOTHING\n", 0, 26);
			}
	
			ofDrawBitmapString("transparency: " + ofToString((int)transparency), 0, 39); 
		
		ofPopMatrix();
	
		ofPopStyle();
	ofPopMatrix();
	
	
}
