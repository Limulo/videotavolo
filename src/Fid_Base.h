/*
 *  Superclasse per tutti gli oggetti di tipo fiducial (siano essi rotativi, quadrati, rotondi, accordi, ecc...).
 *
 *  tutti i fiducial hanno 2 metodi UPDATE:
 *  1) CONTINUOUS
 *  2) INTERRUPT
 *  I metodi di tipo CONTINUOUS sono chiamati
 *  ad ogni ciclo dalla funzione update di ofApp.
 *  I metodi di tipo INTERRUPT sono chiamati
 *  dalla funzione di callback 'objectUpdated' per TUIO
 *
 *	Tutti i fiducial implementano 4 animazioni principali
 *	1) comparsa (quando il fiducial è posizionato sul tavolo);
 *	2) scomparsa (quando il fiducial viene rimosso dalla superficie);
 *	3) espansione (quando la playhead viene a contatto col fiducial provocando il trigger);
 *	4) collasso (quando, raggiunto il picco di espansione massimo, l'animazione deve ritirarsi).
 *
 *	COMPARSA : è regolata dal valore di 'STEP_IN' che è un valore numerico (NON di tempo o frames)
 *	SCOMPARSA : è regolata dal valore di 'STEP_OUT' che è un valore numerico (NON di tempo o frames)
 *  ESPANSIONE : è regolata dal valore 'tExpand', un valore di tempo espresso in frames
 *	COLLASSO : è regolata dal valore 'tCollapse', un valore di tempo espresso in frames
 *
 */

#ifndef _INC_FID_BASE
#define _INC_FID_BASE

#define FIDUCIAL_R		45		// raggio dei fiducial rotondi
#define FIDUCIAL_L		95		// lato dei fiducial quadrati


#define FIDUCIAL_FPS	60
#define STEP_IN			int( 255.0f / ( FIDUCIAL_FPS * (150.0f / 1000.0f) ) ) // 170 ms
#define	STEP_OUT		int( 255.0f / ( FIDUCIAL_FPS * (350.0f / 1000.0f) ) ) // 354 ms

#include <iostream>
#include <math.h>
#include "ofMain.h"

using namespace std;


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
		bool	bAlive;			
		
		// variabili inerenti all'animazione del fiducial al passaggio della playhead
		bool	bExpand;
		bool	bCollapse;
		bool	bTrigger;
		int		marginTrigger;		// marginTrigger rappresenta la distanza tra centro del fiducial e coordinata x della playhead
		unsigned int	tExpand;
		unsigned int	tCollapse;
		unsigned int	startFrame;
		unsigned int	aTrigger;		// ampiezza della figura triggerata
	
		float distanza(ofVec2f *a, ofVec2f *b);
	
		public:
			Fid_Base();
			~Fid_Base() { /*cout << "FID_BASE: De-constructiong!" << endl;*/ }
	
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
