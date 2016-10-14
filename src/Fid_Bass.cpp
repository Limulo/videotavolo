#include "Fid_Bass.h"


// COSTRUTTORE /////////////////////////////////////////////////
Fid_Bass::Fid_Bass(int _fid, int _sid)
{
	//std::cout << "FID BASS: Constructiong derived!" << std::endl;
	fid = _fid;
	sid = _sid;
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
	bAlive = true;
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
	// la variabile A serve per ammorbidire ulteriormente l'animazione in ingresso e in uscita.
	// La grandezza del disegno del basso è data da un valore fisso ( 'FIDUCIAL_R' )
	// e da un valore variabile pari a 'bass_level_ * BASS_MAX_AMP' .
	// La variabile 'A', proporzionale alla trasparenza, la quale varia in accordo con il tempo 
    // di 'Fade in' e 'Fade out' ( cui sono legati anche i valori di 'STEP_IN' e 'STEP_OUT' ), 
	// scala ulterioremente il valore variabile tra 0 e 1.
	// Quando il fiducial è posizionato sulla superficie (ci troviamo in FADE_IN) l'ampiezza del disegno 
	// è pari alla sola ampiezza fissa, anche se il suono del basso è ad un suo picco massimo di ampiezza, 
	// questa viene scalata per la variabile 'A' che, in accordo con il progressivo aumentare 
	// della trasparenza tra 0 e 255, varia gradualmente fino ad assestarsi su 1 nello stato STABLE.
	// In questo stato il rapporto tra dilatazione e suono in ingresso è di 1:1.
	// Il procedimento simile ed opposto si verifica nel caso contrario in cui 'A' varia da 1 a 0 
	// come transparency varia da 255 a 1.
	
	float A = 1.0;
	// STATE UPDATE -----------------------------------------------
	if (stato == FADE_IN) 
	{
		transparency += STEP_IN;
		if(transparency > 255 )
		{ 
			transparency = 255;
			stato = STABLE;
		} 
		A = (transparency / 255.0);
		
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
		A = (transparency / 255.0);
		
	} 
	 
	// qualunque sia lo stato in cui mi trovi, provvedo a calcolare l'ampiezza del disegno sempre basata sul
	// livello sonoro in ingresso. Se sono nello stato STABLE il rapporto tra dilatazione del disegno e 
	// livello sonoro in ingresso è pari a 1:1 mentre è ridotto se siamo in FADE_IN o FADE_OUT
	bass_level = A * bass_level_ * BASS_MAX_AMP * 0.10;
	
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
	
	if(transparency != 0) 
	{
		//l'ampiezza del fiducial varia su base del livello di volume 
		//che viene inviato al metodo 'update_continuos' direttamente da PureData
		
		// FIDUCIAL: disegno del fiducial --------------------------
		ofSetColor(f_color);
		ofFill();
		ofDrawCircle(fid_pos, FIDUCIAL_R + bass_level );
		ofNoFill();
		ofDrawCircle(fid_pos, FIDUCIAL_R + bass_level );
		
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
	return bAlive;
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
	ofDrawBitmapString("BASS\nFid_Pos X: " + ofToString(fid_pos.x) + 
					   "\nFid_Pos Y: " + ofToString(fid_pos.y) + 
					   "\nFid_Angle: " + ofToString(fid_angle), 0, 0);
	ofPopMatrix();
	
	// RIQUADRO 4 -----------------------------------------------
	ofPushMatrix();
	ofTranslate(-50, 80 , 0);
	
	ofDrawBitmapString("f-id: " + ofToString((int)fid) + 
					   ";  s-id: " + ofToString((int)sid), 0, 0);
	
	if (bAlive)
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







