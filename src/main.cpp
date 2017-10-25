/*
* This example is see how a separate thread
* can deal with ofEvents. We noticed a separate thread
* can send notification of an event which happened
* the thing is that every Listener to the event will be
* executed in the same "space" of the thread which
* generated it!.*
*/

#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main()
{

	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 600, 600, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp());
}
