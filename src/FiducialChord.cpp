#include "FiducialChord.h"

FiducialChord::FiducialChord(int fid, int sid, struct SettingsManager::settings *_s): FiducialBase (fid, sid, _s) {
  ofLogVerbose(" fid chord ") << "("<< fid << ", "<< sid << "): constructed";
  loadSettings();
}

FiducialChord::~FiducialChord() {
  ofLogVerbose(" fid chord ") << "("<< fid << ", "<< sid << "): destroyed";
}

void FiducialChord::update() {
  //ofLogVerbose(" fid chord ") << "("<< fid << ", "<< sid << "): update";

  if( asr.update() < 0.0 )
    alive = false;

  y_asr = asr.getY();
  anim.update();
  y_anim = anim.getY();

  size = y_asr * SIZE*(1+y_anim);

  alpha = 255.0 * y_asr;
  //ofLogVerbose(" fid chord ") << "y_asr: "<< y_asr << ", size: "<< size << ", alpha: "<< alpha << ", alive: "<< alive << ")";
}

void FiducialChord::tuioAdd( float _x, float _y, float _angle, float _rspeed ) {
  ofLogVerbose(" fid chord ") << "("<< fid << ", "<< sid << "): tuioAdd";
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

void FiducialChord::tuioUpdate( float _x, float _y, float _angle, float _rspeed ) {
  ofLogVerbose(" fid chord ") << "("<< fid << ", "<< sid << "): tuioUpdate";
  pos.x = _x;
  pos.y = _y;
  angle = _angle;
  rspeed = _rspeed;
}

void FiducialChord::tuioRemove() {
  ofLogVerbose(" fid chord ") << "("<< fid << ", "<< sid << "): tuioRemove";
  asr.triggerRelease();
}


// functions to save and load useful settings for the class
void FiducialChord::saveSettings()
{
  ofLogVerbose(" fid chord ") << "("<< fid << ", "<< sid << "): save Settings";
  // do nothing for the moment
  FiducialBase::saveSettings();
}


void FiducialChord::loadSettings()
{
  ofLogVerbose(" fid chord ") << "("<< fid << ", "<< sid << "): load Settings";

  //FiducialBase::loadSettings();
}

/************************************************************************/
/*                             CHORD 1                                  */
/************************************************************************/
Chord1::Chord1(int fid, int sid, struct SettingsManager::settings *_s): FiducialChord (fid, sid, _s) {
  ofLogVerbose(" fid chord1") << "("<< fid << ", "<< sid << "): constructed!";
  loadSettings();
}

Chord1::~Chord1() {
  ofLogVerbose(" fid chord1") << "("<< fid << ", "<< sid << "): destroyed";
}

void Chord1::draw()
{
  ofPushMatrix();
  ofPushStyle();
  ofSetColor(r, g, b, alpha);

  ofTranslate( pos );
  drawDebug("chord 1");

  ofRotate( angle*360/(2*PI), 0, 0, 1.0);
  ofDrawRectangle( 0.0, 0.0, size, size );

  ofPopStyle();
  ofPopMatrix();
}

// functions to save and load useful settings for the class
void Chord1::saveSettings() {
  ofLogVerbose(" fid chord1")<< "("<< fid << ", "<< sid << "): save Settings";
  // do nothing for the moment
  FiducialChord::saveSettings();
}

void Chord1::loadSettings() {
  ofLogVerbose(" fid chord1") << "("<< fid << ", "<< sid << "): load Settings";

  r = s->fiducials.chords.chord1.color.r;
  g = s->fiducials.chords.chord1.color.g;
  b = s->fiducials.chords.chord1.color.b;

  FiducialChord::loadSettings();
}



/************************************************************************/
/*                             CHORD 2                                  */
/************************************************************************/
Chord2::Chord2(int fid, int sid, struct SettingsManager::settings *_s): FiducialChord (fid, sid, _s) {
  ofLogVerbose(" fid chord2") << "("<< fid << ", "<< sid << "): constructed!";
  loadSettings();
}

Chord2::~Chord2() {
  ofLogVerbose(" fid chord2") << "("<< fid << ", "<< sid << "): destroyed";
}

void Chord2::draw()
{
  ofPushMatrix();
  ofPushStyle();
  ofSetColor(r, g, b, alpha);

  ofTranslate( pos );
  drawDebug("chord 2");

  ofRotate( angle*360/(2*PI), 0, 0, 1.0);
  ofDrawRectangle( 0.0, 0.0, size, size );

  ofPopStyle();
  ofPopMatrix();
}

// functions to save and load useful settings for the class
void Chord2::saveSettings() {
  ofLogVerbose(" fid chord2")<< "("<< fid << ", "<< sid << "): save Settings";
  // do nothing for the moment
  FiducialChord::saveSettings();
}

void Chord2::loadSettings() {
  ofLogVerbose(" fid chord2") << "("<< fid << ", "<< sid << "): load Settings";

  r = s->fiducials.chords.chord2.color.r;
  g = s->fiducials.chords.chord2.color.g;
  b = s->fiducials.chords.chord2.color.b;

  FiducialChord::loadSettings();
}


/************************************************************************/
/*                             CHORD 3                                  */
/************************************************************************/
Chord3::Chord3(int fid, int sid, struct SettingsManager::settings *_s): FiducialChord (fid, sid, _s) {
  ofLogVerbose(" fid chord3") << "("<< fid << ", "<< sid << "): constructed!";
  loadSettings();
}

Chord3::~Chord3() {
  ofLogVerbose(" fid chord3") << "("<< fid << ", "<< sid << "): destroyed";
}

void Chord3::draw()
{
  ofPushMatrix();
  ofPushStyle();
  ofSetColor(r, g, b, alpha);

  ofTranslate( pos );
  drawDebug("chord 3");

  ofRotate( angle*360/(2*PI), 0, 0, 1.0);
  ofDrawRectangle( 0.0, 0.0, size, size );

  ofPopStyle();
  ofPopMatrix();
}

// functions to save and load useful settings for the class
void Chord3::saveSettings() {
  ofLogVerbose(" fid chord3")<< "("<< fid << ", "<< sid << "): save Settings";
  // do nothing for the moment
  FiducialChord::saveSettings();
}

void Chord3::loadSettings() {
  ofLogVerbose(" fid chord3") << "("<< fid << ", "<< sid << "): load Settings";

  r = s->fiducials.chords.chord3.color.r;
  g = s->fiducials.chords.chord3.color.g;
  b = s->fiducials.chords.chord3.color.b;

  FiducialChord::loadSettings();
}


/************************************************************************/
/*                             CHORD 4                                  */
/************************************************************************/
Chord4::Chord4(int fid, int sid, struct SettingsManager::settings *_s): FiducialChord (fid, sid, _s) {
  ofLogVerbose(" fid chord4") << "("<< fid << ", "<< sid << "): constructed!";
  loadSettings();
}

Chord4::~Chord4() {
  ofLogVerbose(" fid chord4") << "("<< fid << ", "<< sid << "): destroyed";
}

void Chord4::draw()
{
  ofPushMatrix();
  ofPushStyle();
  ofSetColor(r, g, b, alpha);

  ofTranslate( pos );
  drawDebug("chord 4");

  ofRotate( angle*360/(2*PI), 0, 0, 1.0);
  ofDrawRectangle( 0.0, 0.0, size, size );

  ofPopStyle();
  ofPopMatrix();
}

// functions to save and load useful settings for the class
void Chord4::saveSettings() {
  ofLogVerbose(" fid chord4")<< "("<< fid << ", "<< sid << "): save Settings";
  // do nothing for the moment
  FiducialChord::saveSettings();
}

void Chord4::loadSettings() {
  ofLogVerbose(" fid chord4") << "("<< fid << ", "<< sid << "): load Settings";

  r = s->fiducials.chords.chord4.color.r;
  g = s->fiducials.chords.chord4.color.g;
  b = s->fiducials.chords.chord4.color.b;

  FiducialChord::loadSettings();
}

/************************************************************************/
/*                             CHORD 5                                  */
/************************************************************************/
Chord5::Chord5(int fid, int sid, struct SettingsManager::settings *_s): FiducialChord (fid, sid, _s) {
  ofLogVerbose(" fid chord5") << "("<< fid << ", "<< sid << "): constructed!";
  loadSettings();
}

Chord5::~Chord5() {
  ofLogVerbose(" fid chord5") << "("<< fid << ", "<< sid << "): destroyed";
}

void Chord5::draw()
{
  ofPushMatrix();
  ofPushStyle();
  ofSetColor(r, g, b, alpha);

  ofTranslate( pos );
  drawDebug("chord 5");

  ofRotate( angle*360/(2*PI), 0, 0, 1.0);
  ofDrawRectangle( 0.0, 0.0, size, size );

  ofPopStyle();
  ofPopMatrix();
}

// functions to save and load useful settings for the class
void Chord5::saveSettings() {
  ofLogVerbose(" fid chord5")<< "("<< fid << ", "<< sid << "): save Settings";
  // do nothing for the moment
  FiducialChord::saveSettings();
}

void Chord5::loadSettings() {
  ofLogVerbose(" fid chord5") << "("<< fid << ", "<< sid << "): load Settings";

  r = s->fiducials.chords.chord5.color.r;
  g = s->fiducials.chords.chord5.color.g;
  b = s->fiducials.chords.chord5.color.b;

  FiducialChord::loadSettings();
}


/************************************************************************/
/*                             CHORD 6                                  */
/************************************************************************/
Chord6::Chord6(int fid, int sid, struct SettingsManager::settings *_s): FiducialChord (fid, sid, _s) {
  ofLogVerbose(" fid chord6") << "("<< fid << ", "<< sid << "): constructed!";
  loadSettings();
}

Chord6::~Chord6() {
  ofLogVerbose(" fid chord6") << "("<< fid << ", "<< sid << "): destroyed";
}

void Chord6::draw()
{
  ofPushMatrix();
  ofPushStyle();
  ofSetColor(r, g, b, alpha);

  ofTranslate( pos );
  drawDebug("chord 6");

  ofRotate( angle*360/(2*PI), 0, 0, 1.0);
  ofDrawRectangle( 0.0, 0.0, size, size );

  ofPopStyle();
  ofPopMatrix();
}

// functions to save and load useful settings for the class
void Chord6::saveSettings() {
  ofLogVerbose(" fid chord6")<< "("<< fid << ", "<< sid << "): save Settings";
  // do nothing for the moment
  FiducialChord::saveSettings();
}

void Chord6::loadSettings() {
  ofLogVerbose(" fid chord6") << "("<< fid << ", "<< sid << "): load Settings";

  r = s->fiducials.chords.chord6.color.r;
  g = s->fiducials.chords.chord6.color.g;
  b = s->fiducials.chords.chord6.color.b;

  FiducialChord::loadSettings();
}
