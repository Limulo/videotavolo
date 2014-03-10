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

// Superclasse per tutti gli oggetti di tipo fiducial (siano essi rotativi, quadrati, rotondi, accordi, ecc...).


#ifndef _INC_FID_BASE
#define _INC_FID_BASE

#define FIDUCIAL_R		45		// raggio dei fiducial rotondi
#define FIDUCIAL_L		95		// lato dei fiducial quadrati

#include <iostream>
#include <math.h>
#include "ofMain.h"

using namespace std;

/*
 *  tutti i fiducial hanno 2 metodi UPDATE:
 *  1) CONTINUOUS
 *  2) INTERRUPT
 *  I metodi di tipo CONTINUOUS sono chiamati
 *  ad ogni ciclo dalla funzione update di testApp.
 *  I metodi di tipo INTERRUPT sono chiamati
 *  dalla funzione di callback 'objectUpdated' per TUIO
 *
 */

class Fid_Base 
{
	protected:
		int fid;					// fiducial ID
		int sid;					// session ID
	
		ofVec2f	fid_pos;			// conserva i dati x e y passati da TUIO (float32, 0-1)
		float	fid_angle;			// conserva il dato di rotazione passato da TUIO (float32, 0-2PI)
		float	fid_prev_angle;		// conserva il precedente dato di rotazione passato da TUIO (float32, 0-2PI)
	
		// variabili usate dalla sola sottoclasse Fid_Rot
		float	fid_cateto;			// differenza tra le ascisse del centro del tavolo e del fiducial
		float	fid_ctr_angle;		// coefficiente angolare della retta congiungente la posizione del fiducial con il centro del tavolo
		float	rel_rot;			// rotazione relativa degli elementi grafici associati al fiducial, calcolato su base di fid_cateto e fid_ctr_angle (GRADI)
	
		ofColor f_color;			// colore di riempimento del fiducial
	
		// enumeratore che etichetta gli stati in cui si viene a trovare il fiducial
		enum State 
		{
			FADE_IN = 0,	
			FADE_OUT,
			STABLE
		} stato;
	
		int		transparency;
		bool	alive;			
		
		// variabili inerenti all'animazione del fiducial al passaggio della playhead
		bool	bExpand;
		bool	bCollapse;
		bool	bTrigger;
		int		marginTrigger;		// marginTrigger rappresenta la distanza tra centro del fiducial e coordinata x della playhead
		unsigned int	tExpand;
		unsigned int	tCollapse;
		unsigned int	startFrame;
		unsigned int aTrigger;		// ampiezza della figura triggerata
	
		float distanza(ofVec2f *a, ofVec2f *b);
	
		public:
			Fid_Base() {cout << "FID_BASE: Constructiong!\n"; }
			~Fid_Base() {cout << "FID_BASE: De-constructiong!\n"; }
	
			virtual void added() = 0;
	 
			virtual void update_interrupt(ofVec2f *_fid_pos, ofVec2f *_ctr_pos, float _fid_angle, float _fid_rot_vel) = 0;
			virtual void update_continuos(int playHeadPos_) = 0;
	 
			virtual void removed() = 0;
	 
			virtual void draw() = 0;
	 
			virtual ofVec2f* getFidPos() = 0;
			virtual bool isAlive() = 0;	
	 
			virtual void inside(ofVec2f *p) = 0;
			virtual void debug() = 0;
	
	
			int get_f_id();
			int get_s_id();
	
			
	
};

#endif
