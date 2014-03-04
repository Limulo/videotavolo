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