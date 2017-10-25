/*
* This class is meant to
* take care of the apperance of the
* interactive surface top.
*
* This class is tought to be used by the
* main (OpenGL) thread only because it uses
* OpenGl methods in order to draw the
* graphics of each fiducial.
*/

#ifndef SURFACE_MANAGER_H_
#define SURFACE_MANAGER_H_

#include "ofMain.h"
#include "TimingThread.h"
#include "Sequencer.h"
#include "OscManager.h"
#include "SettingsManager.h"
#include "FiducialsManager.h"

class SurfaceManager
{
private:
  float currentFPS;
  uint64_t seconds;
  uint64_t minutes;
  uint64_t hours;

  ofMatrix4x4 m;
	ofFbo fbo;

	ofVec2f middle, fboMiddle, playgroundMiddle, playgroundUL;
	float playgroundSize, fboSize;
	float rotX, rotZ;
	float moveX, moveY;
	float scale;

  void calculateTransformation();
  void resetTransformation();

  TimingThread *thread;
  float playheadPosition;
  int index; // where to store the actual column the seq is playing

  struct SettingsManager::settings *s;
  FiducialsManager fiducialsManager;

public:

  SurfaceManager();
  ~SurfaceManager();

  void setup(int _w, int _h,
              struct SettingsManager::settings *_s,
              TimingThread *_t,
              Sequencer *_seq,
              OscManager *_oscMan
            );
  void update();
  void draw();

  void keyPressed( int key );
  void windowResized( int _w, int _h );

  void saveSettings();
  void loadSettings();

  //method below is used by the main thread to have the
  // TUIO Manager get a reference to this object
  FiducialsManager * getFiducialsManager();
};

#endif /* SURFACE_MANAGER_H_ */
