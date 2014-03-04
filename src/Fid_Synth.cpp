/*
 *  Fid_Synth.cpp
 *  Prototipo3
 *
 *  Created by Limulo.
 *  http://www.limulo.net 
 */

#include "Fid_Synth.h"



// COSTRUTTORE /////////////////////////////////////////////////
Fid_Synth::Fid_Synth(int _fid, int _sid)
{
	
	std::cout << "FID SYNTH: Constructing derived!\n" << std::endl;
	
	fid = _fid;
	sid = _sid;
	
	//fid_pos
	fid_pos.set(0.0f, 0.0f);
	transparency = 0;
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
void Fid_Synth::setup(ofVec2f *_fid_pos, ofVec2f *_ctr_pos, float _fid_angle, ofColor _color)
{
	// UPDATE POSITION --------------------------------------------------------------
	fid_pos.set(_fid_pos->x, _fid_pos->y);
	fid_angle = _fid_angle;
	
	r = _color.r;
	g = _color.g;
	b = _color.b;
	
	count = 0;
	r_osc = 0.0;
	
}

// ADDED ///////////////////////////////////////////////////////
void Fid_Synth::added()
{
	stato = FADE_IN;
	alive = true;
}


// UPDATES /////////////////////////////////////////////////////
void Fid_Synth::update_interrupt(ofVec2f *_fid_pos, ofVec2f *_ctr_pos, float _fid_angle, float _fid_rot_vel)
{
	
	// UPDATE POSITION --------------------------------------------------------------
	fid_pos.set(_fid_pos->x, _fid_pos->y);
	fid_angle = _fid_angle;
}

void Fid_Synth::update_continuos(int bpm_) 
{
	float A = 1.0;
	// STATE UPDATE -----------------------------------------------
	if(stato != STABLE) 
	{
		
		if (stato == FADE_IN) 
		{
			transparency += STEP_IN;
			A = (transparency / 255.0);
			if(transparency > 255 )
			{ 
				transparency = 255;
				stato = STABLE;
			} 
			
		} 
		else if (stato == FADE_OUT) 
		{
			transparency -= STEP_OUT;
			A = (transparency / 255.0);
			if (transparency < 0)
			{
				stato = STABLE;
				transparency = 0;
				alive = false;
			} 
		} 
	} 
	else 
	{
		// se sono nello stato STABLE valuto r_osc. Questa variabile contiene un valore che oscilla come un seno
		float f = bpm_ / (4 * 60.0f); // 60 secondi in un minuto
		float t = ( ofGetFrameNum() / 60.0f ); // 60 = è il FPS
		r_osc = A * sin( 2*PI * f * t ); // f *
	}

	f_color.set(r, g, b, transparency);

}

// REMOVED /////////////////////////////////////////////////////
void Fid_Synth::removed(void)
{
	stato = FADE_OUT;
}

// DRAW ////////////////////////////////////////////////////////
void Fid_Synth::draw(void)
{
	ofEnableAlphaBlending();
	ofPushMatrix();
	ofPushStyle();
	ofTranslate(fid_pos);
	ofRotate( (fid_angle* ( 360.0f / (2.0f*PI) )), 0, 0, 1);
	
	float raggio = r_osc * PAD_MAX_AMP;
	if(transparency != 0) {
		
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofSetColor(f_color);
		ofFill();
		ofRect(0, 0, FIDUCIAL_L + raggio, FIDUCIAL_L + raggio);
		ofNoFill();
		ofRect(0, 0, FIDUCIAL_L + raggio, FIDUCIAL_L + raggio);
	}
	
	ofPopStyle();
	ofPopMatrix();
	ofDisableAlphaBlending();
	
}

// GETTERS /////////////////////////////////////////////////////
ofVec2f* Fid_Synth::getFidPos() 
{
	return &fid_pos;
}

bool Fid_Synth::isAlive() 
{
	return alive;
}

// OTHER ///////////////////////////////////////////////////////
void Fid_Synth::inside(ofVec2f *p)
{}

void Fid_Synth::debug() 
{
	
	// TESTO a schermo ------------------------------------------
	ofPushMatrix();
	ofPushStyle();
	ofTranslate(fid_pos);
	
	ofSetHexColor(0x000000);
	
	// RIQUADRO 1 -----------------------------------------------
	ofPushMatrix(); 
	ofTranslate(-50, -120 , 0);
	ofDrawBitmapString("SYNTH\nFid_Pos X: " + ofToString(fid_pos.x) + "\nFid_Pos Y: " + ofToString(fid_pos.y) + "\nFid_Angle: " + ofToString(fid_angle), 0, 0);
	ofPopMatrix();
	
	// RIQUADRO 4 -----------------------------------------------
	ofPushMatrix();
	ofTranslate(-50, 100 , 0);
	
	ofDrawBitmapString("f-id: " + ofToString((int)fid) + ";\t s-id: " + ofToString((int)sid), 0, 0);
	
	if (alive)
		ofDrawBitmapString("alive!\n", 0, 13);
	
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
	//ofDrawBitmapString("raggio: " + ofToString(r_osc * PAD_MAX_AMP), 0, 52);
	
	ofPopMatrix();
	
	ofPopStyle();
	ofPopMatrix();
	
}








