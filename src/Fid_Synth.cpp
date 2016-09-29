#include "Fid_Synth.h"



// COSTRUTTORE /////////////////////////////////////////////////
Fid_Synth::Fid_Synth(int _fid, int _sid)
{
	
	//std::cout << "FID SYNTH: Constructing derived!" << std::endl;
	
	fid = _fid;
	sid = _sid;
	 
   /* 
	* il valore della frequenza è calcolato in modo tale che un intero ciclo di sinusoide si 
	* completi all'interno della battuta ( 4 quarti per un intero ciclo ).
	*
	* | --- 1 BAR --- |
	* |   __          |
	* |  /  \         |
	* | /    \        |
	* +/______\_______| 
	* |        \     /
	* |         \   /
	* |          \_/
	* |
	*
	*/
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
	
	r_osc = 0.0;
	
	t  = 0.0f;
	t0 = 0.0f;
	f  = 0.0;
	float T = 0.0;
	numero_croma = 0;
	
}

// ADDED ///////////////////////////////////////////////////////
void Fid_Synth::added()
{
	stato = FADE_IN;
	bAlive = true;
}


// UPDATES /////////////////////////////////////////////////////
void Fid_Synth::update_interrupt(ofVec2f *_fid_pos, ofVec2f *_ctr_pos, float _fid_angle, float _fid_rot_vel)
{
	
	// UPDATE POSITION --------------------------------------------------------------
	fid_pos.set(_fid_pos->x, _fid_pos->y);
	fid_angle = _fid_angle;
}


void Fid_Synth::update_continuos(float time_) 
{
	// per capire quale sia la funzione della variabile 'A', vedi i commenti della classe Fid_Bass
	float A = 1.0;
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
	} 
	
	// qualunque sia lo stato in cui mi trovi, provvedo a calcolare l'ampiezza del disegno, 
	// variabile secondo il valore del seno	
	
	// se sono nello stato STABLE valuto r_osc. Questa variabile contiene un valore che oscilla come un seno.
	// Questa funzione seno è valutabile solo se si conoscono due variabili principali:
	// 1) il tempo 't', ottenuto come somma di un valore iniziale 't0' e un valore varibile time_ 
	//    (passato alal funzione come argomento);
	// 2) la frequenza 'f';
	// Sia il valore 'f' e conseguentemente 't0' vengono calcolate ad intervalli regolari, una croma dopo l'altra
	// (vedi metodo ofApp::update). L'algoritmo è pensato appositamente per risolvere la "issue01".
	// Con queste nuove modifiche, variando il tempo 'bpm' tramite Fid_Rot, non si verificano bruschi
	// cambiamenti nell'animazione dell'andamento oscillatorio del Fid_Synth.
	// il valore passato come argomento alla funzione update_continuos "time_" è espresso in ms
	// ci occorre esprimerlo in secondi per usarlo nella formula di oscillazione sinusoidale
	t = t0 + (time_/1000);
	r_osc = A * sin( 2*PI * f * t );
	

	f_color.set(r, g, b, transparency);

}

// RESET INTERNAL TIMER ////////////////////////////////////////
void Fid_Synth::reset_internal_timer(int n_crome_) {
	f = synth_bpm / (4.0f * 60.0f); // 60 secondi in un minuto
	if( f == 0)
		f = 0.000001f;
	T = 1.0f/f;
	numero_croma = n_crome_;
	t0 = numero_croma * ( T / 8 );
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
		ofDrawRectangle(0, 0, FIDUCIAL_L + raggio, FIDUCIAL_L + raggio);
		ofNoFill();
		ofDrawRectangle(0, 0, FIDUCIAL_L + raggio, FIDUCIAL_L + raggio);
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
	return bAlive;
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
	ofDrawBitmapString("SYNTH\nFid_Pos X: " + ofToString(fid_pos.x) + 
					   "\nFid_Pos Y: " + ofToString(fid_pos.y) + 
					   "\nFid_Angle: " + ofToString(fid_angle) + 
					   "\nsbpm: " + ofToString(synth_bpm) + 
					   "\nf: " + ofToString(f) +
					   "\nnCrome: " + ofToString(numero_croma), 0, 0);
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
	//ofDrawBitmapString("raggio: " + ofToString(r_osc * PAD_MAX_AMP), 0, 52);
	
	ofPopMatrix();
	
	ofPopStyle();
	ofPopMatrix();
	
}