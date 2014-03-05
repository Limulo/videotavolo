#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

//#define _FULLSCREEN

// variabili statiche
unsigned int	testApp::bpm;				//	valore di BPM (Beats Per Minute - Battiti al minuto)
int				testApp::croma_time;		//	valore temporale che ciascuna croma assume in funzione del BPM
float			testApp::chord_number;		//	indice relativo all'accordo selezionato
int				Fid_Synth::synth_bpm;		//  valore di bpm locale e condiviso tra tutte le istanze di Fid_Synth


#ifdef _LIBPD
float			AppCore::bass_lvl;			//	livello del suono del basso
#endif


int main()
{
    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1280, 1280, OF_WINDOW);
	
	//Se il flag FULLSCREEN è settato, imposta la dimensione e la posizione della finestra
#ifdef _FULLSCREEN
	ofSetWindowPosition(1280, 100);
#endif
	
	ofRunApp(new testApp());
}
