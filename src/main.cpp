#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGlutWindow.h"

// variabili statiche
unsigned int	ofApp::bpm;				//	valore di BPM (Beats Per Minute - Battiti al minuto)
int				ofApp::croma_time;		//	valore temporale che ciascuna croma assume in funzione del BPM
float			ofApp::chord_number;	//	indice relativo all'accordo selezionato
unsigned int	Fid_Synth::synth_bpm;	//  valore di bpm locale e condiviso tra tutte le istanze di Fid_Synth


int main()
{
    ofAppGlutWindow window;
	
	ofSetupOpenGL(&window, 640, 640, OF_WINDOW);
	ofSetWindowPosition(0, 0);
	
	ofRunApp(new ofApp());
}
