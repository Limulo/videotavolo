#include "ofApp.h"
using namespace std;

//--------------------------------------------------------------
void ofApp::setup()
{
  ofSetFrameRate(60);
  ofSetVerticalSync( true );

  // set loggging for each modules
  ofSetLogLevel("    main   ", OF_LOG_NOTICE);
  ofSetLogLevel(" animator  ", OF_LOG_NOTICE);
  ofSetLogLevel(" fid base  ", OF_LOG_NOTICE);
  ofSetLogLevel(" fid perc. ", OF_LOG_NOTICE);
  ofSetLogLevel(" fid kick  ", OF_LOG_NOTICE);
  ofSetLogLevel(" fid snare ", OF_LOG_NOTICE);
  ofSetLogLevel(" fid hihat ", OF_LOG_NOTICE);
  ofSetLogLevel(" fid metro ", OF_LOG_NOTICE);
  ofSetLogLevel(" fid bass  ", OF_LOG_NOTICE);
  ofSetLogLevel(" fid pad   ", OF_LOG_NOTICE);
  ofSetLogLevel(" fid chord ", OF_LOG_NOTICE);
  ofSetLogLevel(" fid chord1", OF_LOG_NOTICE);
  ofSetLogLevel(" fid chord2", OF_LOG_NOTICE);
  ofSetLogLevel(" fid chord3", OF_LOG_NOTICE);
  ofSetLogLevel(" fid chord4", OF_LOG_NOTICE);
  ofSetLogLevel(" fid chord5", OF_LOG_NOTICE);
  ofSetLogLevel(" fid chord6", OF_LOG_NOTICE);
  ofSetLogLevel("  fid  man ", OF_LOG_NOTICE);
  ofSetLogLevel("  osc  man ", OF_LOG_NOTICE);
  ofSetLogLevel("  tuio man ", OF_LOG_NOTICE);
  ofSetLogLevel("  sett man ", OF_LOG_NOTICE);
  ofSetLogLevel("  surf man ", OF_LOG_NOTICE);
  ofSetLogLevel(" sequencer ", OF_LOG_NOTICE);
  ofSetLogLevel(" time thr. ", OF_LOG_NOTICE);
  ofSetLogLevel("   udacn   ", OF_LOG_NOTICE);

  // seconds = 0;
  // minutes = 0;
  // hours = 0;

  // Setting Manager
  settingsManager.setup();
  s = settingsManager.getCustoms();

  // GUI stuff
  //gui.setup("GUI");

  // we create a binding between an event (emitted from the thread)
  // and the corresponding method of the Sequencer
  ofAddListener( thread.ev, &sequencer, &Sequencer::play );

  // then we start the thread
  thread.start( s );
  // thread.lock();
  // int bpm = thread.bpm;
  // thread.unlock();
  // gui.add( intSlider.set("bpm", bpm, s->bpm.min, s->bpm.max) ); //GUI Stuff

  // we create the OSC manager
  oscManager.setup( s );
  sequencer.setup( &oscManager );

  // we create the surface manager
  surfaceManager.setup( ofGetWidth(), ofGetHeight(), s, &thread, &sequencer, &oscManager);

  // we create the TUIO manager
  tuioManager.setup( s, surfaceManager.getFiducialsManager() );

  ofBackground(255, 0, 0);
}

//--------------------------------------------------------------
void ofApp::update()
{
  // //cout << "ofApp update!" << endl;
  // currentFPS = ofGetFrameRate();
  //
  // // how mush time has passed from the moment we
  // // started the application?
  // seconds = ofGetElapsedTimeMillis() / 1000;
  // minutes = seconds / 60;
  // hours   = minutes / 60;
  // minutes = minutes % 60;
  // seconds = seconds % 60;

  // thread.lock();
  // thread.bpm = intSlider;
  // thread.unlock();

  tuioManager.update();

  // it's important to have the OSC manager
  // to receive OSC message before calling
  // update for the surface and for the fiducials.
  oscManager.update();

  surfaceManager.update();

  ofBackground( s->aspect.background.r, s->aspect.background.g, s->aspect.background.b);
}

//--------------------------------------------------------------
void ofApp::draw()
{
  surfaceManager.draw();

  // ofPushStyle();
  // ofSetColor( 0 );
  // int n = surfaceManager.getFiducialsManager()->getFiducialsNumber();
  // string s = "time: " + ofToString(hours) + ":" +
  //                       ofToString(minutes) + ":" +
  //                       ofToString(seconds) + ":" +
  //             "FPS: "+ofToString( currentFPS, 2 ) +"\n"+
  //             "fids: "+ofToString( n );
  // ofDrawBitmapString(s, ofGetWidth()-90, 20);
  // ofPopStyle();

  //gui.draw();
}

//--------------------------------------------------------------
void ofApp::exit() {
  thread.stop();
  ofRemoveListener( thread.ev, &sequencer, &Sequencer::play );
}





// for the moment let's use the keyPressed function to
// simulate the actions of "adding" and "removing"
// fiducials from the surface.
void ofApp::keyPressed(int key)
{
  /*
  if (key == 'a') {
    tuioManager.tuioAdd();
  }
  else if (key == 'r') {
    tuioManager.tuioRemove();
  }
  else if (key == 'c'){
    tuioManager.clear();
  }
  else*/ if (key == OF_KEY_F1 ) {
    ofLogVerbose("    main   ") << "\nSAVE";
    surfaceManager.saveSettings();
    //fiducialManager.saveSettings();
    oscManager.saveSettings();
    // the following function must be
    // the last thing to call.
    settingsManager.saveSettings();
  }
  else if (key == OF_KEY_F5 ) {
    ofLogVerbose("    main   ") << "\nLOAD";
    settingsManager.loadSettings();
    //fiducialsManager.loadSettings();
    surfaceManager.loadSettings();

    oscManager.loadSettings();
    tuioManager.loadSettings();
    // we don't want the bpm can be changed from the
    // XML file. We want this value to be a startup
    // settings only.
    //thread.loadSettings();
  }
  else if (key == OF_KEY_F9 ) {
    settingsManager.loadDefaults();
    //fiducialsManager.loadSettings();
    surfaceManager.loadSettings();
    oscManager.loadSettings();
    tuioManager.loadSettings();
  }
  else if (key == OF_KEY_F11 ) {
    ofToggleFullscreen();
  }
  else if (key == 'o') {
    oscManager.swap();
  }
  else if (key == 't') {
    tuioManager.swap();
  }
  else
  {
    surfaceManager.keyPressed( key );
  }
}

void ofApp::windowResized( int w, int h )
{
  ofLogVerbose("    main   ") << "window resized";
  surfaceManager.windowResized( w, h );
}






// below a mouse function to be used to make better debug
// TODO: eventually use the mouse to have an alternative
// way to add and remove fiducials from the playground.
void ofApp::mousePressed(int x, int y, int button) {
  //cout << "mouse pressed: " << button << ";" << endl;
  //mouseManager.pressed(x, y, button);
  //bool b = mouseManager.inside(x, y);
  //cout << b << endl;
}

void ofApp::mouseReleased(int x, int y, int button) {
  //cout << "mouse released: " << button << ";" << endl;
  //mouseManager.released(x, y, button);
}

void ofApp::mouseMoved(int x, int y) {
  //mouseManager.update(x, y);
}
