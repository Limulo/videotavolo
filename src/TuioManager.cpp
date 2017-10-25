#include "TuioManager.h"
using namespace std;

TuioManager::TuioManager() {
  ofLogVerbose("  tuio man ") << "contructed!";
}

TuioManager::~TuioManager() {
  ofLogVerbose("  tuio man ") << "destroyed!";

  // remember to remove all the listener you have created
  ofRemoveListener(tuioClient.cursorAdded,   this, &TuioManager::cursorAdded);
  ofRemoveListener(tuioClient.cursorRemoved, this, &TuioManager::cursorRemoved);
  ofRemoveListener(tuioClient.cursorUpdated, this, &TuioManager::cursorUpdated);

  ofRemoveListener(tuioClient.objectAdded,   this, &TuioManager::objectAdded);
  ofRemoveListener(tuioClient.objectRemoved, this, &TuioManager::objectRemoved);
  ofRemoveListener(tuioClient.objectUpdated, this, &TuioManager::objectUpdated);
}

void TuioManager::setup(struct SettingsManager::settings *_s, FiducialsManager * _fidMan)
{
  ofLogVerbose("  tuio man ") << "setup";
  s = _s;
  fidMan = _fidMan;

  loadSettings(); // one time loading

  tuioClient.start( LISTEN_PORT );
  ofAddListener(tuioClient.cursorAdded,   this, &TuioManager::cursorAdded);
  ofAddListener(tuioClient.cursorRemoved, this, &TuioManager::cursorRemoved);
  ofAddListener(tuioClient.cursorUpdated, this, &TuioManager::cursorUpdated);

  ofAddListener(tuioClient.objectAdded,   this, &TuioManager::objectAdded);
  ofAddListener(tuioClient.objectRemoved, this, &TuioManager::objectRemoved);
  ofAddListener(tuioClient.objectUpdated, this, &TuioManager::objectUpdated);
}


void TuioManager::update()
{
  //ofLogVerbose("  tuio man ") << "update";
  tuioClient.getMessage();
}


// We want new object to be added and updated to the surface
// only if TUIO STATUS is on!
// But, in order to loose consistency with the actual seurface
// and the internal representation of it, we must let the
// `tuioRemove` method continue to work.
// Indeed problems could arise when we turn the TUIO STATUS off
// and remove a fiducial from the surface. In this case the object
// is still present in the internal representation of the surface
// (sequencer) and it will be never removed, neither if we turn
// the TUIO STATUS on!
void	TuioManager::cursorAdded(ofxTuioCursor & tuioCursor)
{
  if (STATUS)
  {
    ofLogVerbose("  tuio man ") << "cursor added";
    // do something else
  }
}

void	TuioManager::cursorUpdated(ofxTuioCursor & tuioCursor)
{
  if (STATUS)
  {
    ofLogVerbose("  tuio man ") << "cursor updated";
    // do somthing else
  }
}

void	TuioManager::cursorRemoved(ofxTuioCursor & tuioCursor)
{
  ofLogVerbose("  tuio man ") << "cursor removed";
}


void TuioManager::objectAdded(ofxTuioObject & tuioObject)
{
  if (STATUS)
  {
    ofLogVerbose("  tuio man ") << "object added";
    sid    = tuioObject.getSessionId();
    fiducial=tuioObject.getFiducialId();
    xpos   = tuioObject.getX();
    ypos   = tuioObject.getY();
    xspeed = tuioObject.getXSpeed();
    yspeed = tuioObject.getYSpeed();
    mspeed = tuioObject.getMotionSpeed();
    maccel = tuioObject.getMotionAccel();
    angle  = tuioObject.getAngle();
    rspeed = tuioObject.getRotationSpeed();
    raccel = tuioObject.getRotationAccel();

    fidMan->tuioAdd( sid, fiducial, xpos, ypos, angle, rspeed );
  }
}

void	TuioManager::objectUpdated(ofxTuioObject & tuioObject)
{
  if( STATUS )
  {
    ofLogVerbose("  tuio man ") << "object updated";
    sid    = tuioObject.getSessionId();
    fiducial=tuioObject.getFiducialId();
    xpos   = tuioObject.getX();
    ypos   = tuioObject.getY();
    xspeed = tuioObject.getXSpeed();
    yspeed = tuioObject.getYSpeed();
    mspeed = tuioObject.getMotionSpeed();
    maccel = tuioObject.getMotionAccel();
    angle  = tuioObject.getAngle();
    rspeed = tuioObject.getRotationSpeed();
    raccel = tuioObject.getRotationAccel();

    fidMan->tuioUpdate( sid, fiducial, xpos, ypos, angle, rspeed );
  }
}

void	TuioManager::objectRemoved(ofxTuioObject & tuioObject)
{
  ofLogVerbose("  tuio man ") << "object removed";

  sid    = tuioObject.getSessionId();
  fiducial=tuioObject.getFiducialId();
  // xpos   = tuioObject.getX();
  // ypos   = tuioObject.getY();
  // xspeed = tuioObject.getXSpeed();
  // yspeed = tuioObject.getYSpeed();
  // mspeed = tuioObject.getMotionSpeed();
  // maccel = tuioObject.getMotionAccel();
  // angle  = tuioObject.getAngle();
  // rspeed = tuioObject.getRotationSpeed();
  // raccel = tuioObject.getRotationAccel();

  fidMan->tuioRemove( sid, fiducial );
}

/****************************************************************************/

// not used fro the moment
void TuioManager::saveSettings()
{
	ofLogVerbose("  tuio man ") << "save settings";
	// do nothing
}

void TuioManager::loadSettings()
{
	ofLogVerbose("  tuio man ") << "load settings";

	if( s->tuio.status == "on" )
	{
		STATUS = true;
		ofLogVerbose("  tuio man ") << "status ON!";
	}
	else
	{
		STATUS = false;
		ofLogVerbose("  tuio man ") << "status OFF";
	}

	LISTEN_PORT = ofToInt( s->tuio.listen );
}


void TuioManager::swap()
{
	STATUS = !STATUS;
	if( STATUS )
		ofLogVerbose("  tuio man ") << "swap: status ON!";
	else
		ofLogVerbose("  tuio man ") << "swap: status OFF";
}
