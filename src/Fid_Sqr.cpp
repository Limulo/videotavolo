/*
 * videoTavolo
 * ===========
 *
 * INFO
 * ===========
 * videoTavolo è un prototipo di superficie interattiva. 
 * Vengono qui rilasciati i codici sorgente e le patches di PureData del progetto. 
 * Il progetto è curato da Limulo ( http://www.limulo.net ) con i seguenti contributi esterni:
 *
 * 1) ofxPd
 * Copyright (c) Dan Wilcox 2011-2013
 * BSD Simplified License.
 * https://github.com/danomatika/ofxPd
 * 
 * 2) ofxTuio
 * permette di creare e gestire direttamente nell'ambiente 
 * di sviluppo di openFrameworks un server e un client che 
 * comunicano tramite protocollo TUIO
 * https://github.com/patriciogonzalezvivo/ofxTuio
 * 
 * LICENZA
 * ===========
 * ad eccezione degli elementi elencati qui sopra, tutto il codice è rilasciato da Limulo secondo la licenza 
 * Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0). Per prendere visione di una copia 
 * di tale licenza visitate http://creativecommons.org/licenses/by-sa/4.0/ .
 * 
 */

#include "Fid_Sqr.h"


// COSTRUTTORE /////////////////////////////////////////////////
Fid_Sqr::Fid_Sqr(int _fid, int _sid)
{
	
	//std::cout << "FID SQR: Constructing derived!" << std::endl;
	
	fid = _fid;
	sid = _sid;
}


// SETUP ///////////////////////////////////////////////////////
// con il metodo setup per il fiducial rotativo imposto
// 1) la posizione del fiducial
// 2) la posizione del centro del tavolo di gioco (non usata)
// 3) la rotazione del fiducial (dato passato da TUIO)
// 4) il colore del fiducial
void Fid_Sqr::setup(ofVec2f *_fid_pos, ofVec2f *_ctr_pos, float _fid_angle, ofColor _color)
{
	// UPDATE POSITION --------------------------------------------------------------
	fid_pos.set(_fid_pos->x, _fid_pos->y);
	fid_angle = _fid_angle;

	r = _color.r;
	g = _color.g;
	b = _color.b;
	
}

// ADDED ///////////////////////////////////////////////////////
void Fid_Sqr::added()
{
	stato = FADE_IN;
	bAlive = true;
}


// UPDATES /////////////////////////////////////////////////////
void Fid_Sqr::update_interrupt(ofVec2f *_fid_pos, ofVec2f *_ctr_pos, float _fid_angle, float _fid_rot_vel)
{
	// UPDATE POSITION --------------------------------------------------------------
	fid_pos.set(_fid_pos->x, _fid_pos->y);
	fid_angle = _fid_angle;
}

void Fid_Sqr::update_continuos(int playHeadPos_) 
{
	
	// STATE UPDATE -----------------------------------------------
	if(stato != STABLE) {
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
				bAlive = false;
			} 
		}
		
	} 
	else
	{
		// se sono in stato STABLE 
		// se la differenza tra la coordinata x della playhead e il centro del fiducial è minore di marginTrigger
		// il fiducial viene triggerato, modifico quindi le variabili booleane in modo da dare il via all'animazione corrispondente
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

	f_color.set(r, g, b, transparency);
}

// REMOVED /////////////////////////////////////////////////////
void Fid_Sqr::removed(void)
{
	//state_actual = false;
	stato = FADE_OUT;
}

// DRAW ////////////////////////////////////////////////////////
void Fid_Sqr::draw(void)
{
	ofEnableAlphaBlending();
	ofPushMatrix();
	ofPushStyle();
	ofTranslate(fid_pos);
	ofRotate( (fid_angle* ( 360.0f / (2.0f*PI) )), 0, 0, 1);
	
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
				// decadimento cubico (elevo alla terza per dare una maggiore morbidezza all'animazione)
				raggio = ( (tCollapse - (tCollapse - tExpand)) / float(tCollapse) ) * ( (tCollapse - (tCollapse - tExpand)) / float(tCollapse) ) * ( (tCollapse - (tCollapse - tExpand)) / float(tCollapse) ) ;
				if (raggio < 0.01) 
				{
					bCollapse = false;
					
				}
			} 
		}
		
		// FIDUCIAL: disegno del trigger --------------------------
		ofSetColor(ofColor(r, g, b, raggio*255));
		ofFill();
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofRect(0, 0, FIDUCIAL_L + raggio*aTrigger, FIDUCIAL_L + raggio*aTrigger);
		ofNoFill();
		ofRect(0, 0, FIDUCIAL_L + raggio*aTrigger, FIDUCIAL_L + raggio*aTrigger);
		
		// FIDUCIAL: disegno del fiducial --------------------------
		ofSetColor(f_color);
		//ofSetHexColor(0x0000ee);
		ofFill();
		ofRect(0, 0, FIDUCIAL_L, FIDUCIAL_L);
		//ofSetHexColor(0x000000);
		ofSetColor(0, 0, 0, transparency);
		ofNoFill();
		ofRect(0, 0, FIDUCIAL_L, FIDUCIAL_L);
	}
	
	ofPopStyle();
	ofPopMatrix();
	ofDisableAlphaBlending();
		 
}

// GETTERS /////////////////////////////////////////////////////
ofVec2f* Fid_Sqr::getFidPos() 
{
	return &fid_pos;
}

bool Fid_Sqr::isAlive() 
{
	return bAlive;
}

// OTHER ///////////////////////////////////////////////////////
void Fid_Sqr::inside(ofVec2f *p)
{}

void Fid_Sqr::debug() 
{
	
	// TESTO a schermo ------------------------------------------
	ofPushMatrix();
		ofPushStyle();
		ofTranslate(fid_pos);
	
		ofSetHexColor(0x000000);
	
		// RIQUADRO 1 -----------------------------------------------
		ofPushMatrix(); 
			ofTranslate(-50, -120 , 0);
			ofDrawBitmapString("FIDUCIAL QUADRATO\nFid_Pos X: " + ofToString(fid_pos.x) + "\nFid_Pos Y: " + ofToString(fid_pos.y) + "\nFid_Angle: " + ofToString(fid_angle), 0, 0);
		ofPopMatrix();
	
		// RIQUADRO 4 -----------------------------------------------
		ofPushMatrix();
			ofTranslate(-50, 100 , 0);
	
			ofDrawBitmapString("f-id: " + ofToString((int)fid) + ";\t s-id: " + ofToString((int)sid), 0, 0);
	
			if (bAlive)
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
	
		ofPopMatrix();
	
		ofPopStyle();
	ofPopMatrix();
	 
}








