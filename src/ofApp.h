/*
* se the image to see how the app works
*/

#pragma once

#include "ofMain.h"
//#include "ofxGui.h"

#include "SettingsManager.h"
#include "TimingThread.h"
#include "Sequencer.h"
#include "OscManager.h"
#include "SurfaceManager.h"
#include "TuioManager.h"
#include "udacn.h"

class ofApp : public ofBaseApp
{
private:
	//struct SurfaceManager::UI *i; // no more used

public:
	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);
	void windowResized(int w, int h);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseMoved(int x, int y);

	//ofxPanel gui;
	//ofParameter<int> intSlider;
	//ofxIntSlider intSlider;
	//ofxToggle toggle;
	//ofxButton button;

	SettingsManager settingsManager;
	struct SettingsManager::settings *s;

	TimingThread thread;
	OscManager oscManager;
	Sequencer sequencer;
	SurfaceManager surfaceManager;
	//FiducialsManager fiducialsManager;

	TuioManager tuioManager;
	//MouseManager mouseManager;

	int width, height;
	// float currentFPS;
	// uint64_t seconds;
	// uint64_t minutes;
	// uint64_t hours;
};
