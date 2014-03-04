/*
 *  Fid_Round.cpp
 *  Prototipo3
 *
 *  Created by Limulo.
 *  http://www.limulo.net 
 */

#include "Fid_Round.h"


// COSTRUTTORE /////////////////////////////////////////////////
Fid_Round::Fid_Round(int _fid, int _sid)
{
	std::cout << "FID ROUND: Constructiong derived!\n" << std::endl;
	
	fid = _fid;
	sid = _sid;
	
	c_innerRadius = FIDUCIAL_R;
		
	fid_pos.set(0.0f, 0.0f);
	fid_ctr_angle = 0.0f;
	
	z_axis.set(0, 0, 1);
	
	transparency	= 0;
	stato = STABLE;
	
	alive = false;
	
	// variabili utili per l'animazione del trigger
	// ossia quando la playhead raggiunge la posizione del fiducial
	bTrigger = false;
	bExpand = false;
	bCollapse = false;
	tExpand = 10;		// valore temporale (espresso in frames) per l'espansione
	tCollapse = 480;	// valore temporale (espresso in frames) per il ritorno alle normali dimensioni
	aTrigger = 80;		// ampiezza di dilatazione della forma del fiducial quando venga triggerato
	startFrame = 0;
	marginTrigger = 30;	// raggio di un cerchio immaginario, tracciato contrandolo sulla posizione del fiducial. 
	
}


// SETUP ///////////////////////////////////////////////////////
void Fid_Round::setup(ofVec2f *_fid_pos, ofVec2f *_ctr_pos, float _fid_angle, ofColor _fColor)
{
	// SETUP POSITION --------------------------------------------------------------
	fid_pos.set(_fid_pos->x, _fid_pos->y);
	fid_angle = _fid_angle;
	ofSetCircleResolution(CIRCLE_RES);
	
	fr = _fColor.r;
	fg = _fColor.g;
	fb = _fColor.b;
}

// ADDED ///////////////////////////////////////////////////////
void Fid_Round::added(void)
{
	stato = FADE_IN;
	alive = true;
}


// UPDATES /////////////////////////////////////////////////////
void Fid_Round::update_interrupt(ofVec2f *_fid_pos, ofVec2f *_ctr_pos, float _fid_angle, float _fid_rot_vel)
{
	
	// UPDATE POSITION --------------------------------------------------------------
	fid_pos.set(_fid_pos->x, _fid_pos->y);
	fid_angle = _fid_angle;
	
}

void Fid_Round::update_continuos(int playHeadPos_) 
{
	
	// STATE UPDATE -----------------------------------------------
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
			alive = false;
		} 
		
	} 
	else 
	{
		if(!bTrigger && abs((fid_pos.x - playHeadPos_)) < marginTrigger) 
		{
			bTrigger = true;
			bExpand = true;
			startFrame = ofGetFrameNum();
		} 
		else if (bTrigger && !bExpand && !bCollapse && abs((fid_pos.x - playHeadPos_)) > marginTrigger) 
		{
			bTrigger = false;
		}
	}
	
	
	f_color.set(fr, fg, fb, transparency);
}



// REMOVED /////////////////////////////////////////////////////
void Fid_Round::removed(void)
{
	stato = FADE_OUT;
}


// DRAW ////////////////////////////////////////////////////////
void Fid_Round::draw(void)
{
	ofPushStyle();
	ofEnableAlphaBlending();
	
	float raggio = 0.0;
	if(transparency != 0) 
	{
		if(bTrigger) 
		{
			int t = ofGetFrameNum() - startFrame;
			
			if(bExpand) 
			{
				raggio = (t / float(tExpand) );
				if (raggio > 0.9) 
				{
					bExpand = false;
					bCollapse = true;
				}
			} 
			else if (bCollapse) 
			{
				// decadimento cubico
				raggio = ( (tCollapse - (tCollapse - tExpand)) / float(tCollapse) ) * ( (tCollapse - (tCollapse - tExpand)) / float(tCollapse) ) * ( (tCollapse - (tCollapse - tExpand)) / float(tCollapse) ) ;
				if (raggio < 0.01) 
				{
					bCollapse = false;
					
				}
			} 
		}
		
		
		// FIDUCIAL: disegno del trigger --------------------------
		ofSetColor(f_color);
		ofFill();
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofCircle(fid_pos, FIDUCIAL_R + raggio*aTrigger);
		ofNoFill();
		ofCircle(fid_pos, FIDUCIAL_R + raggio*aTrigger);
		
		
		// FIDUCIAL: disegno del fiducial --------------------------
		ofSetColor(f_color);
		//ofSetHexColor(0x0000ee);
		ofFill();
		ofCircle(fid_pos, FIDUCIAL_R);
		ofSetHexColor(0x000000);
		ofNoFill();
		ofCircle(fid_pos, FIDUCIAL_R);
		
	}
	
	ofDisableAlphaBlending();
	ofPopStyle();
}


// GETTERS /////////////////////////////////////////////////////
ofVec2f* Fid_Round::getFidPos() 
{
	return &fid_pos;
}

bool Fid_Round::isAlive() 
{
	return alive;
}



// OTHER ///////////////////////////////////////////////////////
void Fid_Round::inside(ofVec2f *p)
{
	// non usato
}



void Fid_Round::debug() {
	
	// TESTO a schermo ------------------------------------------
	ofPushMatrix();
	ofPushStyle();
	ofTranslate(fid_pos);
	
	ofSetHexColor(0x000000);
	
	// RIQUADRO 1 -----------------------------------------------
	ofPushMatrix(); 
	ofTranslate(-50, -120 , 0);
	ofDrawBitmapString("FIDUCIAL ROTONDO (kick)\nFid_Pos X: " + ofToString(fid_pos.x) + "\nFid_Pos Y: " + ofToString(fid_pos.y) + "\nFid_Angle: " + ofToString(fid_angle), 0, 0);
	ofPopMatrix();
	
	 
	// RIQUADRO 4 -----------------------------------------------
	ofPushMatrix();
	ofTranslate(-50, 80 , 0);
	
	ofDrawBitmapString("f-id: " + ofToString((int)fid) + ";\t s-id: " + ofToString((int)sid), 0, 0);
	
	if (alive)
		ofDrawBitmapString("alive!\n", 0, 13);
	
	switch (stato){
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







