#include "FiducialPercussion.h"
//using namespace std;

FiducialPercussion::FiducialPercussion(int fid, int sid, struct SettingsManager::settings *_s): FiducialBase (fid, sid, _s) {
  ofLogVerbose(" fid perc. ") << "("<< fid << ", "<< sid << "): constructed";
}

FiducialPercussion::~FiducialPercussion() {
  ofLogVerbose(" fid perc. ") << "("<< fid << ", "<< sid << "): destroyed";
}


void FiducialPercussion::update()
{
  //ofLogVerbose(" fid perc. ") << "FIDUCIAL_PERCUSSION ("<< fid << ", "<< sid << "): update";

  ar.update();
  if( asr.update() < 0.0 )
    alive = false;

  y_ar = ar.getY();
  y_asr = asr.getY();

  size = SIZE * (1.0 + y_ar) * y_asr;

  alpha = 255.0 * y_asr;
}

void FiducialPercussion::tuioAdd( float _x, float _y, float _angle, float _rspeed )
{
  ofLogVerbose(" fid perc. ") << "("<< fid << ", "<< sid << ")";

  pos.x = _x;
  pos.y = _y;
  angle = _angle;
  rspeed = _rspeed;

  asr.setup( FADEIN_TIME, FADEOUT_TIME );
  ar.setup( TRIGGERIN_TIME, TRIGGEROUT_TIME );
  y_asr = y_ar = 0.0;

  alive = true;
  asr.triggerAttack();
}

void FiducialPercussion::tuioUpdate( float _x, float _y, float _angle, float _rspeed  )
{
  ofLogVerbose(" fid perc. ") << "("<< fid << ", "<< sid << ")";
  // 1) update the position & angle of the fiducial
  pos.x = _x;
  pos.y = _y;
  angle = _angle;
  rspeed = _rspeed;
}


void FiducialPercussion::tuioRemove()
{
  ofLogVerbose(" fid perc. ") << "("<< fid << ", "<< sid << "): tuioRemove ";
  asr.triggerRelease();
}


void FiducialPercussion::setPosInBar( int _posInBar )
{
  ofLogVerbose(" fid perc. ") << "("<< fid << ", "<< sid << "): set Pos in Bar ( posInBar: "<< _posInBar << ")";
  posInBar = _posInBar;
}

int FiducialPercussion::getPosInBar() {
  return posInBar;
}


// this function is called when the playhead is on the fiducial
// and the correspongin sound/animation must be triggered
void FiducialPercussion::trigger()
{
  //ofLogVerbose(" fid perc. ") << "FIDUCIAL_PERCUSSION ("<< fid << ", "<< sid << "): trigger";

  // TODO: it would be better not to see
  // expansion/contraction of the fiducial
  // when it is in fade out
  // if( something )
  ar.trigger();
}



// functions to save and load useful settings for the class
void FiducialPercussion::saveSettings()
{
  ofLogVerbose(" fid perc. ") << "("<< fid << ", "<< sid << "): save Settings";
  // do nothing for the moment
  FiducialBase::saveSettings();
}


void FiducialPercussion::loadSettings()
{
  ofLogVerbose(" fid perc. ") << "("<< fid << ", "<< sid << "): load Settings";

  TRIGGERIN_TIME  = s->fiducials.percussions.triggertime.in;
  TRIGGEROUT_TIME = s->fiducials.percussions.triggertime.out;

  //FiducialBase::loadSettings();
}


/************************************************************************/
/*                              KICK                                    */
/************************************************************************/
Kick::Kick(int fid, int sid, struct SettingsManager::settings *_s): FiducialPercussion (fid, sid, _s) {
  ofLogVerbose(" fid kick  ") << "("<< fid << ", "<< sid << "): constructed!";
  loadSettings();
}

Kick::~Kick() {
  ofLogVerbose(" fid kick  ") << "("<< fid << ", "<< sid << "): destroyed";
}

void Kick::draw()
{
  ofPushMatrix();
  ofPushStyle();
  ofSetColor(r, g, b, alpha);

  ofTranslate( pos );
  drawDebug("kick");

  ofRotate( angle*360/(2*PI), 0, 0, 1.0);
  ofDrawEllipse( 0.0, 0.0, size, size );

  ofPopStyle();
  ofPopMatrix();
}

// functions to save and load useful settings for the class
void Kick::saveSettings() {
  ofLogVerbose(" fid kick  ")<< "("<< fid << ", "<< sid << "): save Settings";
  // do nothing for the moment
  FiducialPercussion::saveSettings();
}

void Kick::loadSettings() {
  ofLogVerbose(" fid kick  ") << "("<< fid << ", "<< sid << "): load Settings";

  r = s->fiducials.percussions.kick.color.r;
  g = s->fiducials.percussions.kick.color.g;
  b = s->fiducials.percussions.kick.color.b;

  FiducialPercussion::loadSettings();
}




/************************************************************************/
/*                             SNARE                                    */
/************************************************************************/
Snare::Snare(int fid, int sid, struct SettingsManager::settings *_s): FiducialPercussion (fid, sid, _s) {
  ofLogVerbose(" fid snare ") << "("<< fid << ", "<< sid << "): constructed";
  loadSettings();
}

Snare::~Snare() {
  ofLogVerbose(" fid snare ") << "("<< fid << ", "<< sid << "): destroyed";
}

void Snare::draw()
{
  ofPushMatrix();
  ofPushStyle();
  ofSetColor(r, g, b, alpha);

  ofTranslate( pos );
  drawDebug("snare");

  ofRotate( angle*360/(2*PI), 0, 0, 1.0);
  ofDrawRectangle( 0.0, 0.0, size, size );

  ofPopStyle();
  ofPopMatrix();
}

// functions to save and load useful settings for the class
void Snare::saveSettings() {
  ofLogVerbose(" fid snare ") << "("<< fid << ", "<< sid << "): save Settings";
  // do nothing for the moment
  FiducialPercussion::saveSettings();
}

void Snare::loadSettings() {
  ofLogVerbose(" fid snare ") << "("<< fid << ", "<< sid << "): load Settings";

  r = s->fiducials.percussions.snare.color.r;
  g = s->fiducials.percussions.snare.color.g;
  b = s->fiducials.percussions.snare.color.b;

  FiducialPercussion::loadSettings();
}





/************************************************************************/
/*                             HIHAT                                    */
/************************************************************************/
Hihat::Hihat(int fid, int sid, struct SettingsManager::settings *_s): FiducialPercussion (fid, sid, _s) {
  ofLogVerbose(" fid hihat ") << "("<< fid << ", "<< sid << "): constructed";
  loadSettings();
}

Hihat::~Hihat() {
  ofLogVerbose(" fid hihat ") << "("<< fid << ", "<< sid << "): destroyed";
}

void Hihat::draw()
{
  ofPushMatrix();
  ofPushStyle();
  ofSetColor(r, g, b, alpha);

  ofTranslate(pos);
  drawDebug("hihat");

  ofRotate( angle*360/(2*PI), 0, 0, 1.0);
  ofDrawRectangle( 0.0, 0.0, size, size );

  ofPopStyle();
  ofPopMatrix();
}

// functions to save and load useful settings for the class
void Hihat::saveSettings() {
  ofLogVerbose(" fid hihat ") << "("<< fid << ", "<< sid << "): save Settings";
  // do nothing for the moment
  FiducialPercussion::saveSettings();
}

void Hihat::loadSettings() {
  ofLogVerbose(" fid hihat ") << "("<< fid << ", "<< sid << "): load Settings";

  r = s->fiducials.percussions.hihat.color.r;
  g = s->fiducials.percussions.hihat.color.g;
  b = s->fiducials.percussions.hihat.color.b;

  FiducialPercussion::loadSettings();
}
