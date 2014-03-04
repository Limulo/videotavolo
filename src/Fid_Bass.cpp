/*
 *  Fid_Bass.cpp
 *  Prototipo3
 *
 *  Created by Limulo.
 *  http://www.limulo.net 
 */

#include "Fid_Bass.h"


// COSTRUTTORE /////////////////////////////////////////////////
Fid_Bass::Fid_Bass(int _fid, int _sid)
{
	std::cout << "FID BASS: Constructiong derived!\n" << std::endl;
	
	fid = _fid;
	sid = _sid;
	
	c_innerRadius = FIDUCIAL_R;
	bass_level = 0.0;
	fid_pos.set(0.0f, 0.0f);
		
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
	aTrigger = 40;		// ampiezza di dilatazione della forma del fiducial quando venga triggerato
	startFrame = 0;
	marginTrigger = 30;	// raggio di un cerchio immaginario, tracciato contrandolo sulla posizione del fiducial. 
	
}


// SETUP ///////////////////////////////////////////////////////
void Fid_Bass::setup(ofVec2f *_fid_pos, ofVec2f *_ctr_pos, float _fid_angle, ofColor _fColor)
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
void Fid_Bass::added(void)
{
	stato = FADE_IN;
	alive = true;
}


// UPDATES /////////////////////////////////////////////////////
void Fid_Bass::update_interrupt(ofVec2f *_fid_pos, ofVec2f *_ctr_pos, float _fid_angle, float _fid_rot_vel)
{
	// UPDATE POSITION --------------------------------------------------------------
	fid_pos.set(_fid_pos->x, _fid_pos->y);
	fid_angle = _fid_angle;
	
}

void Fid_Bass::update_continuos(int bass_level_) 
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
		
	} else if (stato == FADE_OUT) 
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
		// se sono in stato STABLE modifico il valore bass_level in accordo qon il valore ricevuto dall'esterno
		bass_level = bass_level_ * BASS_MAX_AMP * 0.10;
	}
	
	f_color.set(fr, fg, fb, transparency);
}



// REMOVED /////////////////////////////////////////////////////
void Fid_Bass::removed(void)
{
	//state_actual = false;
	stato = FADE_OUT;
}


// DRAW ////////////////////////////////////////////////////////
void Fid_Bass::draw(void)
{
	ofPushStyle();
	ofEnableAlphaBlending();
	
	float raggio = 0.0;
	if(transparency != 0) 
	{
		//l'ampiezza del fiducial varia su base del livello di volume che viene inviato al metodo 'update_continuos' direttamente da PureData
		
		// FIDUCIAL: disegno del fiducial --------------------------
		ofSetColor(f_color);
		ofFill();
		ofCircle(fid_pos, FIDUCIAL_R + bass_level );
		ofNoFill();
		ofCircle(fid_pos, FIDUCIAL_R + bass_level );
		
	}
	
	ofDisableAlphaBlending();
	ofPopStyle();
}


// GETTERS /////////////////////////////////////////////////////
ofVec2f* Fid_Bass::getFidPos() 
{
	return &fid_pos;
}

bool Fid_Bass::isAlive() 
{
	return alive;
}



// OTHER ///////////////////////////////////////////////////////
void Fid_Bass::inside(ofVec2f *p)
{
	// non usato
}



void Fid_Bass::debug() {
	
	// TESTO a schermo ------------------------------------------
	ofPushMatrix();
	ofPushStyle();
	ofTranslate(fid_pos);
	
	ofSetHexColor(0x000000);
	
	// RIQUADRO 1 -----------------------------------------------
	ofPushMatrix(); 
	ofTranslate(-50, -120 , 0);
	ofDrawBitmapString("BASS\nFid_Pos X: " + ofToString(fid_pos.x) + "\nFid_Pos Y: " + ofToString(fid_pos.y) + "\nFid_Angle: " + ofToString(fid_angle), 0, 0);
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







