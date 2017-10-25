#include "FiducialBase.h"
//using namespace std;

FiducialBase::FiducialBase(int _fid, int _sid, struct SettingsManager::settings *_s)
{
  ofLogVerbose(" fid base  ") << "("<< _fid << ", "<< _sid << "): constructed!";
  fid = _fid;
  sid = _sid;
  s = _s; // get the reference to the settings

  // initialize all the settings
  pos = ofVec2f(0.0, 0.0);
  angle   = 0.0;
  rspeed  = 0.0;
  y_asr   = 0.0;
  size    = 0.0;
  alpha   = 0.0;

  // load base settings
  loadSettings();
}

FiducialBase::~FiducialBase() {
  ofLogVerbose(" fid base  ") << "("<< fid << ", "<< sid << "): destroyed";
}

// this is an utility function used to print
// some text near the fiducial graphic representation
// in order to have a quick glance on what is going on
void FiducialBase::drawDebug(string name)
{
  string s =name + "\n"+
            "f:"+ofToString(fid)+", s:"+ofToString(sid)+"\n" +
            "pos ("+ofToString(int(pos.x))+","+ofToString(int(pos.y))+")\n" +
            "angle: "+ofToString(angle)+";\n"+
            "rspeed: "+ofToString(rspeed)+";";
  ofDrawBitmapString(s, SIZE*0.75, -SIZE);
}




// these two function are (eventually) called by the derived classes
void FiducialBase::saveSettings() {
  ofLogVerbose(" fid base  ") << "("<< fid << ", "<< sid << "): save Settings";
  // do nothing for the moment
}

void FiducialBase::loadSettings() {
  ofLogVerbose(" fid base  ") << "("<< fid << ", "<< sid << "): load Settings";
  FADEIN_TIME = s->fiducials.fadetime.in;
  FADEOUT_TIME = s->fiducials.fadetime.out;
  SIZE = s->fiducials.size;
}




// this are alterante methods to add and remove fiducials
// in order to have another way to debug the application
// bool FiducialBase::inside( float _x, float _y )
// {
//   ofVec2f v = ofVec2f(_x, _y);
//   float d = pos.distance(v);
//   if( d < SIZE )
//     return true;
//   return false;
// }
