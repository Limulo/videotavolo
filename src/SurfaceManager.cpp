#include "SurfaceManager.h"
//using namespace std;

#define PLAYGROUND_MODE
//#define CALIBRATION_MODE


SurfaceManager::SurfaceManager() {
	ofLogVerbose("  surf man ") << "contructed!";
}

SurfaceManager::~SurfaceManager() {
	ofLogVerbose("  surf man ") << "destroyed";
}


void SurfaceManager::setup(int _windowWidth, int _windowHeight,
														struct SettingsManager::settings *_s,
														TimingThread *_t,
														Sequencer *_seq,
														OscManager *_oscMan
													)
{
	ofLogVerbose("  surf man ") << "setup!";

	currentFPS = 0.0;
	seconds = 0;
	minutes = 0;
	hours = 0;

	playgroundSize = 500;
  fboSize = playgroundSize * 3.0 / 2.0;
  fbo.allocate( fboSize, fboSize, GL_RGB);
  //fboSize.set( fboSize, fboSize );
  fboMiddle.set(fboSize*0.5, fboSize*0.5);
  playgroundMiddle.set(playgroundSize*0.5, playgroundSize*0.5);
  playgroundUL.set( (fboSize-playgroundSize)*0.5, (fboSize-playgroundSize)*0.5 );
  middle.set( _windowWidth*0.5, _windowHeight*0.5 );

	// get a reference to the app settings
	s = _s;

	// get a reference to the thread
	thread = _t;

	// we create the fiducials manager
	fiducialsManager.setup(playgroundSize, playgroundSize,
													s,
													thread,
													_seq,
													_oscMan
												);

	moveX = moveY = rotX = rotZ = 0.0;
	scale = 1.0;
	loadSettings();
}

void SurfaceManager::update()
{
	//ofLogverbose("  surf man ") << "update";

  currentFPS = ofGetFrameRate();

  // how mush time has passed from the moment we
  // started the application?
  seconds = ofGetElapsedTimeMillis() / 1000;
  minutes = seconds / 60;
  hours   = minutes / 60;
  minutes = minutes % 60;
  seconds = seconds % 60;

	thread->lock();
	index = thread->index_8n;
	playheadPosition = thread->pos;
	thread->unlock();
	//ofLogVerbose("  surf man ") << "playhead pos = " << playheadPosition << ";"<< endl;

	fiducialsManager.update( index );
}

void SurfaceManager::draw()
{
	//ofLogverbose("  surf man ") << "draw";

	// fill the fbo with graphics --------------------------------
  fbo.begin();


  // clear the buffer (fill the fbo)
  ofClear( s->aspect.fbo.r, s->aspect.fbo.g, s->aspect.fbo.b );
  ofSetCircleResolution(32);

  // draw the playground
  ofPushMatrix();
	ofPushStyle();

	ofTranslate( playgroundUL );

  //ofSetColor( s->aspect.playground.r, s->aspect.playground.g, s->aspect.playground.b ); // not yet used
	ofSetColor( s->aspect.fbo.r, s->aspect.fbo.g, s->aspect.fbo.b );
  ofDrawRectangle(0, 0, playgroundSize, playgroundSize);

	// draw some debug test inside the fbo
	ofPushStyle();
	ofSetColor( 0 );
	int n = getFiducialsManager()->getFiducialsNumber();
	string text = "time: " + ofToString(hours) + ":" +
												ofToString(minutes) + ":" +
												ofToString(seconds) + "\n" +
							"FPS: "+ofToString( currentFPS, 2 ) +"\n"+
							"fids: "+ofToString( n ) + "\n" +
							"bpm: "+ofToString( fiducialsManager.getBpm() ) + ";" ;
	ofDrawBitmapString(text, playgroundSize-120, 20);
	ofPopStyle();


#ifdef CALIBRATION_MODE
  // create an 6x6 grid
  ofSetColor(120,120,120);
  float d = playgroundSize / 6.0;
  for(int i=0; i<=6; i++)
  {
    ofDrawLine(i*d, 0, i*d, playgroundSize);
    ofDrawLine(0, i*d, playgroundSize, i*d);
  }
	// create the three circles
	ofSetColor(0, 0, 0);
	ofNoFill();
	ofDrawCircle(playgroundMiddle, d );
	ofDrawCircle(playgroundMiddle, d*2.0 );
	ofDrawCircle(playgroundMiddle, d*3.0 );

#else
	// create the 8x8 grid
	ofSetColor(120,120,120);
	float d = playgroundSize / 8.0;
	for(int i=0; i<=8; i++)
	{
		ofDrawLine(i*d, 0, i*d, playgroundSize);
		ofDrawLine(0, i*d, playgroundSize, i*d);
	}
#endif


	// draw the playhead
	float x = playheadPosition * playgroundSize;
	//cout << playheadPosition << endl;
	float y = 0.0; //height - 30;
	//ofSetColor( 120, 120, 120 );
	//ofDrawLine(width*0.5-108, y, width*0.5+108, y); // horizontal line
	ofSetColor( s->aspect.playhead.r, s->aspect.playhead.g, s->aspect.playhead.b );
	ofDrawLine( x, y, x, y+playgroundSize ); // vertical line
	ofPopStyle();

	// draw the fiducials
	fiducialsManager.draw();
  ofPopMatrix();

  // mark the center of the fbo
  ofSetColor(255, 0, 0);
  ofDrawLine(0, fboMiddle.y, fboSize, fboMiddle.y);
  ofDrawLine(fboMiddle.x, 0, fboMiddle.x, fboSize);

	// // draw the playhead
	// float x = playgroundUL.x + playheadPosition * playgroundSize;
	// float y = playgroundUL.y; //height - 30;
	// //ofSetColor( 120, 120, 120 );
	// //ofDrawLine(width*0.5-108, y, width*0.5+108, y); // horizontal line
	// ofSetColor( interface.playhead_color );
	// ofDrawLine( x, y, x, y+playgroundSize ); // vertical line


  fbo.end();

  // draw the fbo to the screen --------------------------------
  ofPushMatrix();
	ofPushStyle();
  ofMultMatrix( m );
  fbo.getTexture().draw(-fboMiddle.x, -fboMiddle.y);
	ofPopStyle();
  ofPopMatrix();




  // mark the center of the screen -----------------------------
  ofPushMatrix();
  ofPushStyle();
  ofSetColor(0, 255, 0, 100);
  ofDrawLine(0, ofGetHeight()*0.5, ofGetWidth(), ofGetHeight()*0.5);
  ofDrawLine(ofGetWidth()*0.5, 0, ofGetWidth()*0.5, ofGetHeight() );
  ofPopStyle();
  ofPopMatrix();
}

void SurfaceManager::saveSettings()
{
	ofLogVerbose("  surf man ") << "save settings";
	//ofLogVerbose("  surf man ") << "move("<< moveX << ", " << moveY << ") "
	//														<< "rotX: " << rotX << "; scale: " << scale << ";";
	s->aspect.distorsion.moveX = moveX;
	s->aspect.distorsion.moveY = moveY;
	s->aspect.distorsion.rotX  = rotX;
	s->aspect.distorsion.rotZ  = rotZ;
	s->aspect.distorsion.scale = scale;
	fiducialsManager.saveSettings();
}

void SurfaceManager::loadSettings()
{
	ofLogVerbose("  surf man ") << "load settings";
	moveX = s->aspect.distorsion.moveX;
	moveY = s->aspect.distorsion.moveY;
	rotX  = s->aspect.distorsion.rotX;
	rotZ  = s->aspect.distorsion.rotZ;
	scale = s->aspect.distorsion.scale;
	calculateTransformation();
	fiducialsManager.loadSettings();
}

FiducialsManager * SurfaceManager::getFiducialsManager()
{
	return &fiducialsManager;
}

void SurfaceManager::calculateTransformation()
{
	ofLogVerbose("  surf man ") << "calculate transformation";

  // A part from an initial translation to move
  // the graphics to the middle of the screen,
  // there are some operations we need to apply
	// a conter correct distorsion.
  // Here's the complete sequence of transformations:
  // 1) initial translation to the `middle` position of the screen;
  // 2) rotation on X of an angle of `alpha`;
	// 3) possible rotation along Z axis;
  // 4) translation along X and Y axis;
  // 5) possibly a scale transformation;
  // So the transformation matrix must do all these
  // transformation in the reverse order.

  m.makeIdentityMatrix();

  m.scale( scale, scale, scale );

  m.translate(moveX, moveY, 0.0);

	m.rotate(rotZ, 0.0, 0.0, 1.0);

  // rotate(angle degree, x axis, y axis, z axis);
  m.rotate(rotX, 1.0, 0.0, 0.0);
  //m.rotate(rotY, 0.0, 1.0, 0.0);
  //m.rotate(rotZ, 0.0, 0.0, 1.0);

  m.translate( middle );
}

void SurfaceManager::resetTransformation()
{
	ofLogVerbose("  surf man ") << "reset transformation";

  rotX = rotZ = 0.0;
  moveX = moveY = 0.0;
  scale = 1.0;
  calculateTransformation();
}

void SurfaceManager::keyPressed( int key )
{
	if( key == 'q') {
    rotX += 0.5;
    calculateTransformation();
  }
  else if( key == 'e') {
    rotX -= 0.5;
    calculateTransformation();
  }
	else if( key == '1') {
    rotZ += 0.5;
    calculateTransformation();
  }
  else if( key == '3') {
    rotZ -= 0.5;
    calculateTransformation();
  }
  else if( key == 'a') {
    moveX += 1;
    calculateTransformation();
  }
  else if( key == 'd') {
    moveX -= 1;
    calculateTransformation();
  }
  else if( key == 'w') {
    moveY += 1;
    calculateTransformation();
  }
  else if( key == 's') {
    moveY -= 1;
    calculateTransformation();
  }
  else if( key == 'z') {
    scale += 0.01;
    calculateTransformation();
  }
  else if( key == 'x') {
    scale -= 0.01;
    calculateTransformation();
  }
  else if( key == OF_KEY_F7) {
    resetTransformation();
  }
}

void SurfaceManager::windowResized( int _w, int _h )
{
	ofLogVerbose("  surf man ") << "window resized";
  middle.set( _w*0.5, _h*0.5 );
  calculateTransformation();
}
