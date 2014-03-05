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

#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

#define _FULLSCREEN

// variabili statiche
unsigned int	testApp::bpm;				//	valore di BPM (Beats Per Minute - Battiti al minuto)
int				testApp::croma_time;		//	valore temporale che ciascuna croma assume in funzione del BPM
float			testApp::chord_number;		//	indice relativo all'accordo selezionato


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
