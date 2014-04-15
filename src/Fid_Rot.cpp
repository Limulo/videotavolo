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

#include "Fid_Rot.h"


// COSTRUTTORE /////////////////////////////////////////////////
Fid_Rot::Fid_Rot(int _fid, int _sid)
{
	std::cout << "FID ROT: Constructiong derived!\n" << std::endl;
	
	fid = _fid;
	sid = _sid;
	
	fid_ctr_angle = 0.0f;
	z_axis.set(0, 0, 1); // setto l'asse zeta
	c_fill_angle = 0.5f; // l'angolo di riempimento è inzializzato a 0.5f inzialmente
	c_innerRadius = FIDUCIAL_R + CORONA_OFF;		// inizializzo la dimensione per il raggio interno della corona circolare
	c_outerRadius = (CORONA_L) + c_innerRadius;		// inizializzo la dimensione per il raggio esterno della corona circolare
	
}


// SETUP ///////////////////////////////////////////////////////
// con il metodo setup per il fiducial rotativo imposto
// 1) la posizione del fiducial
// 2) la posizione del centro del tavolo di gioco (su base della quale si applicherà una rotazione relativa per la corona circolare) (non usata nel metodo setup)
// 3) la rotazione del fiducial (dato passato da TUIO)
// 4) l'ultimo angolo memorizzato dall'azione dell'ultimo fiducial rotativo presente sul tavolo
// 5) il colore del fiducial
// 6) il colore di riempimento della corona circolare
void Fid_Rot::setup(ofVec2f *_fid_pos, ofVec2f *_ctr_pos, float _fid_angle, float _lim_angle, ofColor _fColor, ofColor _cColor)
{
	// SETUP POSITION --------------------------------------------------------------
	fid_pos.set(_fid_pos->x, _fid_pos->y);
	fid_angle = _fid_angle;
	
	fid_limited_angle = _lim_angle;
	
	fr = _fColor.r;
	fg = _fColor.g;
	fb = _fColor.b;
	
	cr = _cColor.r;
	cg = _cColor.g;
	cb = _cColor.b;
	
	ofSetCircleResolution(CORONA_RES);
	
}

// ADDED ///////////////////////////////////////////////////////
void Fid_Rot::added(void)
{
	stato = FADE_IN;
	alive = true;
}


// UPDATES /////////////////////////////////////////////////////
void Fid_Rot::update_interrupt(ofVec2f *_fid_pos, ofVec2f *_ctr_pos, float _fid_angle, float _fid_rot_vel)
{
	
	// UPDATE POSITION --------------------------------------------------------------
	fid_pos.set(_fid_pos->x, _fid_pos->y);
	fid_angle = _fid_angle;
	
	// UPDATE ROTATION rispetto al CENTRO TAVOLO ------------------------------------
	// usiamo Pitagora per calcolare l'angolo di rotazione da applicare agli elementi grafici
	// su base della posizione relativa del fiducial rispetto al centro del tavolo
	fid_cateto = (fid_pos.x - _ctr_pos->x);
	fid_ctr_angle = acos( fid_cateto / sqrt(distanza(&fid_pos, _ctr_pos)) ) ;
	if(fid_pos.y > _ctr_pos->y) 
	{
		fid_ctr_angle = 2.0f*PI - fid_ctr_angle;
	}
	fid_ctr_angle *= ( 360.0f / (2.0f*PI) );	// conversione da radianti a gradi
	rel_rot = (-1)*fid_ctr_angle + 180 - (CORONA_A/2.0f);
	
	
	
	// UPDATE ROTATION for CORONA FILLING -------------------------------------------
	// QUI DI SEGUITO UN ALGORITMO che permette di valutare le variabili necessarie
	// per rendere graficamente il riempimento di una corona circolare
	// in base al movimento rotatorio applicato al fiducial.
	// più si ruota il fiducial in una certa direzione (e sempre nella stessa)
	// più la corana si riempirà/svuoterà di colore.
	// le variabili implicate in questo algoritmo sono:
	// fid_angle, fid_prev_angle, fid_limited_angle ed infine c_fill_angle.
	if (_fid_rot_vel > 0)
	{
		if (fid_angle > fid_prev_angle)
			fid_limited_angle += (fid_angle - fid_prev_angle);
		else if (fid_angle < fid_prev_angle)
			fid_limited_angle += ((fid_angle + 2*PI) - fid_prev_angle);
	}
	else if (_fid_rot_vel < 0)
	{
		if (fid_angle < fid_prev_angle)
			fid_limited_angle -= (fid_prev_angle - fid_angle);
		else if (fid_angle > fid_prev_angle)
			fid_limited_angle -= ((fid_prev_angle + 2*PI) - fid_angle);
	}
	fid_prev_angle = fid_angle; // memorizzo il fid_angle in fid_prev_angle per la prossima valutazione
	
	if (fid_limited_angle > FIDUCIAL_MER)
		fid_limited_angle = FIDUCIAL_MER;
	else if (fid_limited_angle < -FIDUCIAL_MER) 
		fid_limited_angle = -FIDUCIAL_MER;	
	
	// Mappiamo fid_limited_angle per FILLARE la CORONA
	c_fill_angle = ofMap( fid_limited_angle , -FIDUCIAL_MER, FIDUCIAL_MER, 0.05f, CORONA_A, true);
	
}

void Fid_Rot::update_continuos(int playHeadPos_) {
	
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
	
	c_color.set(cr, cg, cb, transparency);
	f_color.set(fr, fg, fb, transparency);
	
	// PATH C_FILL ------------------------------------------------
	// disegno la corona riempita di colore
	c_fill.clear();
	c_fill.setFillColor(c_color);
	c_fill.setFilled(true);
	c_fill.setArcResolution(CORONA_RES);
	c_fill.setMode(c_fill.POLYLINES); 
	c_fill.arc(0, 0, c_outerRadius, c_outerRadius, 0, c_fill_angle); 
	c_fill.arcNegative(0, 0, c_innerRadius, c_innerRadius, c_fill_angle, 0);
	c_fill.simplify(0.3);
	c_fill.close();
	
	// PATH C_BORDO -----------------------------------------------
	// disegno il bordo ti tutta la corona
	c_bordo.clear();
	c_bordo.setStrokeColor(0x000000);
	c_bordo.setStrokeWidth(0.7f);
	c_bordo.setFilled(false);
	c_bordo.setArcResolution(CORONA_RES);
	c_bordo.setMode(c_bordo.POLYLINES);
	c_bordo.arc(0, 0, c_outerRadius, c_outerRadius, 0, CORONA_A); 
	c_bordo.arcNegative(0, 0, c_innerRadius, c_innerRadius, CORONA_A, 0);
	c_bordo.simplify(0.3);
	c_bordo.close();
	
	// effettuo le operazioni geometriche di traslazione e rotazione
	// alle due figure appena disegnate
	c_fill.rotate(rel_rot, z_axis);
	c_bordo.rotate(rel_rot, z_axis);
	c_fill.translate(fid_pos);
	c_bordo.translate(fid_pos);
	
}



// REMOVED /////////////////////////////////////////////////////
void Fid_Rot::removed(void)
{
	stato = FADE_OUT;
}


// DRAW ////////////////////////////////////////////////////////
void Fid_Rot::draw(void)
{
	ofPushStyle();
	ofEnableAlphaBlending();
	
	if(transparency != 0) 
	{
		// FIDUCIAL: disegno del fiducial --------------------------
		ofSetColor(f_color);
		ofFill();
		ofCircle(fid_pos, FIDUCIAL_R);
		ofSetHexColor(0x000000);
		ofNoFill();
		ofCircle(fid_pos, FIDUCIAL_R);
	
		// FIDUCIAL: disegno della corona ---------------------------
		c_fill.draw(); //ha il colore già impostato dall'update
		ofSetColor(0, 0, 0, transparency);
		c_bordo.getOutline().begin()->draw();
	}
		
	ofDisableAlphaBlending();
	ofPopStyle();
}


// GETTERS /////////////////////////////////////////////////////
ofVec2f* Fid_Rot::getFidPos() 
{
	return &fid_pos;
}

bool Fid_Rot::isAlive() 
{
	return alive;
}

float Fid_Rot::get_lim_angle() 
{
	return fid_limited_angle;
}


// OTHER ///////////////////////////////////////////////////////
void Fid_Rot::inside(ofVec2f *p)
{
	if( c_bordo.getOutline().begin()->inside(*p) ) 
	{
		c_color.set(0, 255, 0, transparency);
	} else 
	{
		c_color.set(255, 0, 0, transparency);
	}
	
}



void Fid_Rot::debug() {
	// questo metodo ha il compito di mostrare a schermo le informazioni di debug
	// quando si entri nella modalità corrispettiva
	
	// TESTO a schermo ------------------------------------------
	ofPushMatrix();
		ofPushStyle();
		ofTranslate(fid_pos);
	
		ofSetHexColor(0x000000);
	
		// RIQUADRO 1 -----------------------------------------------
		ofPushMatrix(); 
			ofTranslate(-50, -180 , 0);
			ofDrawBitmapString("FIDUCIAL ROTATIVO (BPM)\nFid_Pos X: " + ofToString(fid_pos.x) + "\nFid_Pos Y: " + ofToString(fid_pos.y) + "\nFid_Angle: " + ofToString(fid_angle), 0, 0);
		ofPopMatrix();
	
		// RIQUADRO 2 -----------------------------------------------
		ofPushMatrix();
			ofTranslate(-50, -100 , 0);
			ofDrawBitmapString("fid_cateto: " + ofToString((int)fid_cateto) + "\nfid_ctr_angle: " + ofToString((int)fid_ctr_angle) + "\nrel_rot:" + ofToString((int)rel_rot), 0, 0);
		ofPopMatrix();
	
		// RIQUADRO 3 -----------------------------------------------
		ofPushMatrix();
			ofTranslate(-50, 100 , 0);
			ofDrawBitmapString("fid_limited_angle: " + ofToString((float)fid_limited_angle) + "\nc_fill_angle: " + ofToString((int)c_fill_angle), 0, 0);
		ofPopMatrix();
	
		// RIQUADRO 4 -----------------------------------------------
		ofPushMatrix();
			ofTranslate(-50, 150 , 0);

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







