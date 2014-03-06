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

/*
 * Copyright (c) 2011 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxPd for documentation
 *
 */


#pragma once

#include "ofMain.h"
#include "ofxPd.h"

// a namespace for the Pd types
using namespace pd;

class AppCore : public PdReceiver, public PdMidiReceiver {

public:

	// funzioni generiche
	void setup(const int numOutChannels, const int numInChannels, const int sampleRate, const int ticksPerBuffer);
	void update();
	void draw();
	void exit();
	
	// do something
	//void playTone(int pitch);
	
	// input callbacks
	//void keyPressed(int key);
	
	// funzioni audio di callback
	void audioReceived(float * input, int bufferSize, int nChannels);
	void audioRequested(float * output, int bufferSize, int nChannels);
	
	// -- funzioni di callback di PD ----------------------------------
	// pd message receiver callbacks
	void print(const std::string& message); //void printReceived(const std::string& message); // per ofxPD 0062
	
	//void receiveBang(const std::string& dest);
	void receiveFloat(const std::string& dest, float value); // void floatReceived(const std::string& dest, float value); // per ofxPD 0062
	//void receiveSymbol(const std::string& dest, const std::string& symbol);
	//void receiveList(const std::string& dest, const List& list);
	//void receiveMessage(const std::string& dest, const std::string& msg, const List& list);
	
	// pd midi receiver callbacks
	//void receiveNoteOn(const int channel, const int pitch, const int velocity);
	//void receiveControlChange(const int channel, const int controller, const int value);
	//void receiveProgramChange(const int channel, const int value);
	//void receivePitchBend(const int channel, const int value);
	//void receiveAftertouch(const int channel, const int value);
	//void receivePolyAftertouch(const int channel, const int pitch, const int value);
	
	//void receiveMidiByte(const int port, const int byte);
	// -- fine dei callback -------------------------------------------
	
	ofxPd pd;
	
	/*
	void processEvents(); 	// demonstrates how to manually poll for messages // da esempio ofxPD 0074
	vector<float> scopeArray;		// da esempio ofxPD 0062 e ofxPD 0074
	vector<Patch> instances;		// da esempio ofxPD 0074
	int midiChan;					// da esempio ofxPD 0074
	*/
	 
	// da prototipo 3
	void bang(const std::string& receive);
	void send_float(const std::string& receive, float val);
	static float bass_lvl;
	
};