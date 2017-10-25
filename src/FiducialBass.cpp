#include "FiducialBass.h"

FiducialBass::FiducialBass(int fid, int sid, struct SettingsManager::settings *_s): FiducialBase (fid, sid, _s) {
  ofLogVerbose(" fid bass  ") << "("<< fid << ", "<< sid << "): constructed";
  loadSettings();
}

FiducialBass::~FiducialBass() {
  ofLogVerbose(" fid bass  ") << "("<< fid << ", "<< sid << "): destroyed";
}

void FiducialBass::update() {
  //ofLogVerbose(" fid bass  ") << "("<< fid << ", "<< sid << "): update";

  if( asr.update() < 0.0 )
    alive = false;

  y_asr = asr.getY();

  size = y_asr * SIZE *(1 + level*100.0);

  alpha = 255.0 * y_asr;
  //ofLogVerbose(" fid bass  ") << "y_asr: "<< y_asr << ", size: "<< size << ", alpha: "<< alpha << ", alive: "<< alive << ")";
}

void FiducialBass::setLevel( float _level )
{
  level = _level;
  if( level < 0.0)
    level = 0.0;
  else if( level > 1.0)
    level = 1.0;
  ofLogVerbose(" fid bass  ") << "level: "<< level << ";";
}



void FiducialBass::draw() {
  //ofLogVerbose(" fid bass  ") << "("<< fid << ", "<< sid << "): draw";
  // ofPushStyle();
  // ofSetColor(r, g, b, alpha);
  // ofDrawEllipse( pos, size, size );
  //
  // string s = "bass [fid:"+ofToString(fid)+", sid:"+ofToString(sid)+"]\n("+ofToString(int(pos.x))+","+ofToString(int(pos.y))+")";
  // ofDrawBitmapString(s, pos.x+size*0.5, pos.y-size*0.5);
  //
  // ofPopStyle();

  ofPushMatrix();
  ofPushStyle();
  ofSetColor(r, g, b, alpha);

  ofTranslate( pos );
  drawDebug("bass");

  ofRotate( angle*360/(2*PI), 0, 0, 1.0);
  ofDrawEllipse( 0.0, 0.0, size, size );

  ofPopStyle();
  ofPopMatrix();
}

void FiducialBass::tuioAdd( float _x, float _y, float _angle, float _rspeed ) {
  ofLogVerbose(" fid bass  ") << "("<< fid << ", "<< sid << "): tuioAdd";
  pos.x = _x;
  pos.y = _y;
  angle = _angle;
  rspeed = _rspeed;
  level = 0.0;

  asr.setup( FADEIN_TIME, FADEOUT_TIME );
  y_asr = 0.0;

  alive = true;
  asr.triggerAttack();
}

void FiducialBass::tuioUpdate( float _x, float _y, float _angle, float _rspeed ) {
  ofLogVerbose(" fid bass  ") << "("<< fid << ", "<< sid << "): tuioUpdate";
  pos.x = _x;
  pos.y = _y;
  angle = _angle;
  rspeed = _rspeed;
}

void FiducialBass::tuioRemove() {
  ofLogVerbose(" fid bass  ") << "("<< fid << ", "<< sid << "): tuioRemove";
  asr.triggerRelease();
}


// functions to save and load useful settings for the class
void FiducialBass::saveSettings()
{
  ofLogVerbose(" fid bass  ") << "("<< fid << ", "<< sid << "): save Settings";
  // do nothing for the moment
  FiducialBase::saveSettings();
}


void FiducialBass::loadSettings()
{
  ofLogVerbose(" fid bass  ") << "("<< fid << ", "<< sid << "): load Settings";

  r = s->fiducials.bass.color.r;
  g = s->fiducials.bass.color.g;
  b = s->fiducials.bass.color.b;

  //FiducialBase::loadSettings();
}
