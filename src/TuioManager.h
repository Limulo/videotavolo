#ifndef TUIO_MANAGER_H_
#define TUIO_MANAGER_H_

#include "ofMain.h"
#include "ofxTuio.h"
#include "FiducialsManager.h"
#include "SettingsManager.h"

class TuioManager
{
private:

  FiducialsManager * fidMan;
  struct SettingsManager::settings * s;

  ofxTuioClient tuioClient;

  bool STATUS; //on-off
  int LISTEN_PORT;

  // value used for the TUIO objs
  long sid;
	int fiducial;
  float xpos, ypos; // (float32, 0-1)
	float angle; // (float32, 0-2PI)
	float xspeed, yspeed, mspeed;
	float rspeed;
	float maccel;
	float raccel;

  // value used for the TUIO cursors
  //long sid;
	//float xpos ypos;
	//float xspeed, yspeed;
	//float maccel;

public:
  TuioManager();
  ~TuioManager();

  void setup( struct SettingsManager::settings *s, FiducialsManager * fidMan );
  void update();

  void cursorAdded(ofxTuioCursor & tuioCursor);
  void cursorUpdated(ofxTuioCursor & tuioCursor);
  void cursorRemoved(ofxTuioCursor & tuioCursor);

  void objectAdded(ofxTuioObject & tuioObject);
  void objectUpdated(ofxTuioObject & tuioObject);
  void objectRemoved(ofxTuioObject & tuioObject);

  // function to save & load useful setting for
  // the class/subclass.
  void saveSettings();
  void loadSettings();

  void swap();

};

#endif /* TUIO_MANAGER_H_ */
