#include "FiducialMetro.h"

FiducialMetro::FiducialMetro(int fid, int sid, struct SettingsManager::settings *_s,
                            Udacn* _udacn,
                            float playgroundWidth, float playgroundHeight,
                            float _angleGap
                          ) : FiducialBase (fid, sid, _s)
{
  ofLogVerbose(" fid metro ") << "("<< fid << ", "<< sid << "): constructed";

  // Get a reference to the udacn tool
  // it will be useful to obtain information
  // about the actual accumulation (see udacn documentation)
  udacn = _udacn;
  angleGap = _angleGap;

  playgroundMiddle = ofVec2f( playgroundWidth*0.5, playgroundHeight*0.5 );
  beta = 0.0;

  // create the meter
  meter = new Meter( s, &playgroundMiddle, angleGap );

  loadSettings();
}

// the following contrusctor isn't used.
// we only use the one we can pass a Udacn reference with
FiducialMetro::FiducialMetro(int fid, int sid, struct SettingsManager::settings *_s): FiducialBase (fid, sid, _s) {
  //ofLogVerbose(" fid metro ") << "("<< fid << ", "<< sid << "): constructed";
  //loadSettings();
  //meter = new Meter( s, &playgroundMiddle );
}

FiducialMetro::~FiducialMetro() {
  delete meter ;
  ofLogVerbose(" fid metro ") << "("<< fid << ", "<< sid << "): destroyed";
}

void FiducialMetro::update()
{
  //ofLogVerbose(" fid metro ") << "("<< fid << ", "<< sid << "): update";

  if( asr.update() < 0.0 )
    alive = false;

  y_asr = asr.getY();

  size = SIZE * y_asr;

  alpha = 255.0 * y_asr;

  // Update the meter in order for that
  // to appear and disappeas exactly as
  // the 'metro' fiducial it is associated to.
  meter->update( y_asr );
}

void FiducialMetro::draw()
{
  //ofLogVerbose(" fid metro ") << "("<< fid << ", "<< sid << "): draw";

  ofPushMatrix();


  ofPushStyle();

  // mark the center of the playground
  ofSetColor(255, alpha);
  ofDrawEllipse( playgroundMiddle.x, playgroundMiddle.y, 10, 10 );
  ofDrawLine( playgroundMiddle, pos );

  ofSetColor(r, g, b, alpha);
  ofTranslate( pos );
  drawDebug("bpm");

  ofPushMatrix();
  ofRotate( angle*360/(2*PI), 0, 0, 1.0);
  //ofDrawEllipse( 0.0, 0.0, size, size );
  ofDrawRectangle( 0.0, 0.0, size, size );
  ofPopMatrix();
  ofPopStyle();

  ofRotate( beta - angleGap*0.5 );
  meter->draw();


  ofPopMatrix();
}

void FiducialMetro::tuioAdd( float _x, float _y, float _angle, float _rspeed )
{
  ofLogVerbose(" fid metro ") << "("<< fid << ", "<< sid << "): tuioAdd";
  pos.x = _x;
  pos.y = _y;
  angle = _angle;
  rspeed = _rspeed;

  asr.setup( FADEIN_TIME, FADEOUT_TIME );
  y_asr = 0.0;

  alive = true;
  asr.triggerAttack();

  // Everytime the metro fiducial is placed/moved
  // on the interactive surface, we must figure
  // out where it is in relation with the playground center.
  ofVec2f link = playgroundMiddle - pos;
  beta = ofVec2f(1.0, 0.0).angle( link );

  // When the 'metro' fiducial is created/added
  // is time to create/add also the corresponding meter!
  // Besides position and dimensions, we must also provide
  // the current udacn normalized output.
  meter->tuioAdd(pos.x, pos.y, udacn->getOutput() );
}

void FiducialMetro::tuioUpdate( float _x, float _y, float _angle, float _rspeed )
{
  //ofLogVerbose(" fid metro ") << "("<< fid << ", "<< sid << "): tuioUpdate";
  pos.x = _x;
  pos.y = _y;
  angle = _angle;
  rspeed = _rspeed;

  // Everytime the metro fiducial is placed/moved
  // on the interactive surface, we must figure
  // out where it is in relation with the playground center.
  ofVec2f link = playgroundMiddle - pos;
  beta = ofVec2f(1.0, 0.0).angle( link );

  // Every time the 'metro' fiducial is updated
  // (i.e. its position and rotation change on the surface)
  // we must update also the corresponding meter.
  // The only value we must provide to the meter is (for the moment)
  // the normalized level we obtain from the udacn tool.
  meter->tuioUpdate( pos.x, pos.y, udacn->getOutput() );
}

void FiducialMetro::tuioRemove()
{
  ofLogVerbose(" fid metro ") << "("<< fid << ", "<< sid << "): tuioRemove";
  asr.triggerRelease();
}

// functions to save and load useful settings for the class
void FiducialMetro::saveSettings() {
  ofLogVerbose(" fid metro ") << "("<< fid << ", "<< sid << "): save Settings";
  // do nothing for the moment
  meter->saveSettings();
  FiducialBase::saveSettings();
}

void FiducialMetro::loadSettings() {
  ofLogVerbose(" fid metro ") << "("<< fid << ", "<< sid << "): load Settings";

  r = s->fiducials.metro.color.r;
  g = s->fiducials.metro.color.g;
  b = s->fiducials.metro.color.b;

  meter->loadSettings();
}


/************************************************************************/
/*                              METER                                   */
/************************************************************************/

Meter::Meter( struct SettingsManager::settings *_s, ofVec2f *_playgroundMiddle, float _angleGap )
{
  ofLogVerbose(" fid metro ") << "meter constructed";

  s = _s;

  playgroundMiddle = _playgroundMiddle;
  angleGap = _angleGap;
  resolution = 32;

  loadSettings();
}

Meter::~Meter() {
  ofLogVerbose(" fid metro ") << "meter destroyed";
}

void Meter::update( float _y_asr )
{
  //ofLogVerbose(" fid metro ") << "meter update";
  y_asr = _y_asr;
  alpha = 255.0 * y_asr;
}

void Meter::draw()
{
  ofPushStyle();
  //ofLogVerbose(" fid metro ") << "meter draw";
  backPath.setFillColor( ofColor(backColor, alpha) );
  backPath.draw();
  frontPath.setFillColor( ofColor(frontColor, alpha) );
  frontPath.draw();
	//frontPath.getOutline().begin()->draw();
  ofPopStyle();
}

void Meter::tuioAdd( float _x, float _y, float _level)
{
  ofLogVerbose(" fid metro ") << "meter tuio add";
  pos.set( _x, _y );

  level = _level;

  calculateBackPath();
  calculateFrontPath( level );
}



void Meter::tuioUpdate( float _x, float _y, float _level  )
{
  ofLogVerbose(" fid metro ") << "meter tuio update";

  pos.set( _x, _y );
  level = _level;

  // there's no need to calculate the back path again
  //calculateBackPath();
  calculateFrontPath( level );
}

void Meter::calculateBackPath()
{
  float inRadius = radius;
  float outRadius = radius + w;

  backPath.clear();
  //backPath.setStrokeColor( bc );
  //backPath.setStrokeWidth(0.7f);
  backPath.setFilled(true);
  //backPath.setFillColor( ofColor(120, 120, 120) );
  backPath.setCircleResolution( resolution );
  backPath.setMode( ofPath::POLYLINES );
  backPath.arc(        0, 0, outRadius, outRadius, 0, angleGap );
  backPath.arcNegative(0, 0, inRadius,  inRadius,  angleGap, 0);
  backPath.close();
}

void Meter::calculateFrontPath( float _level )
{
  float inRadius = radius;
  float outRadius = radius + w;

  // After scaled the normalized level to the correct angle range
  // we must add a little amount in order not to
  // brake the ofPath when the amplitude is 0.0
  float levelAngle = _level * angleGap + 0.01; // expressed in degrees

  frontPath.clear();
  frontPath.setFilled(true);
  //frontPath.setFillColor( ofColor(255, 0, 0) );
  frontPath.setCircleResolution( resolution );
  frontPath.setMode( ofPath::POLYLINES );
  frontPath.arc(        0, 0, outRadius, outRadius, 0, levelAngle );
  frontPath.arcNegative(0, 0, inRadius,  inRadius,  levelAngle, 0);
  frontPath.close();
}


// functions to save and load useful settings for the class
void Meter::saveSettings() {
  ofLogVerbose(" fid metro ") << "meter save Settings";
  // do nothing for the moment
}

void Meter::loadSettings() {
  ofLogVerbose(" fid metro ") << "meter load Settings";

  //angleGap = s->fiducials.metro.meter.angleGap * PI;
  //angleGap = (angleGap*360.0) / (2*PI);

  //angleRange = s->fiducials.metro.meter.angleRange * PI;
  //angleRange = (angleRange*360.0) / (2*PI);

  radius = s->fiducials.metro.meter.radius;
  w = s->fiducials.metro.meter.w;

  backColor.r = s->fiducials.metro.meter.backColor.r;
  backColor.g = s->fiducials.metro.meter.backColor.g;
  backColor.b = s->fiducials.metro.meter.backColor.b;

  frontColor.r = s->fiducials.metro.meter.frontColor.r;
  frontColor.g = s->fiducials.metro.meter.frontColor.g;
  frontColor.b = s->fiducials.metro.meter.frontColor.b;
}
