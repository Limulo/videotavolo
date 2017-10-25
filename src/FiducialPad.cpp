#include "FiducialPad.h"

FiducialPad::FiducialPad(int fid, int sid, struct SettingsManager::settings *_s): FiducialBase (fid, sid, _s) {
  ofLogVerbose(" fid pad   ") << "("<< fid << ", "<< sid << "): constructed";
  loadSettings();
}

FiducialPad::~FiducialPad() {
  ofLogVerbose(" fid pad   ") << "("<< fid << ", "<< sid << "): destroyed";
}

void FiducialPad::update() {
  //ofLogVerbose(" fid pad   ") << "("<< fid << ", "<< sid << "): update";

  if( asr.update() < 0.0 )
    alive = false;

  y_asr = asr.getY();
  anim.update();
  y_anim = anim.getY();

  size = y_asr * SIZE*(1+y_anim);
  alpha = 255.0 * y_asr;

  //ofLogVerbose(" fid pad   ") << "y_asr: "<< y_asr << ", size: "<< size << ", alpha: "<< alpha << ", alive: "<< alive << ")";
}

void FiducialPad::draw()
{
  //ofLogVerbose(" fid pad   ") << "("<< fid << ", "<< sid << "): draw";

  ofPushMatrix();
  ofPushStyle();
  ofSetColor(r, g, b, alpha);

  ofTranslate( pos );
  drawDebug("pad");

  ofRotate( angle*360/(2*PI), 0, 0, 1.0);
  ofDrawRectangle( 0.0, 0.0, size, size );

  ofPopStyle();
  ofPopMatrix();
}

void FiducialPad::tuioAdd( float _x, float _y, float _angle, float _rspeed ) {
  ofLogVerbose(" fid pad   ") << "("<< fid << ", "<< sid << "): tuioAdd";
  pos.x = _x;
  pos.y = _y;
  angle = _angle;
  rspeed = _rspeed;

  asr.setup( FADEIN_TIME, FADEOUT_TIME );
  y_asr = 0.0;

  alive = true;
  asr.triggerAttack();

  y_anim = 0.0;
  anim.setup();
}

void FiducialPad::tuioUpdate( float _x, float _y, float _angle, float _rspeed ) {
  ofLogVerbose(" fid pad   ") << "("<< fid << ", "<< sid << "): tuioUpdate";
  pos.x = _x;
  pos.y = _y;
  angle = _angle;
  rspeed = _rspeed;
}

void FiducialPad::tuioRemove() {
  ofLogVerbose(" fid pad   ") << "("<< fid << ", "<< sid << "): tuioRemove";
  asr.triggerRelease();
}


// functions to save and load useful settings for the class
void FiducialPad::saveSettings()
{
  ofLogVerbose(" fid pad   ") << "("<< fid << ", "<< sid << "): save Settings";
  // do nothing for the moment
  FiducialBase::saveSettings();
}


void FiducialPad::loadSettings()
{
  ofLogVerbose(" fid pad   ") << "("<< fid << ", "<< sid << "): load Settings";

  r = s->fiducials.pad.color.r;
  g = s->fiducials.pad.color.g;
  b = s->fiducials.pad.color.b;
}
