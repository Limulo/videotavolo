/*
* This class is meant to
* take care of all the fiducials we
* are usign on the interactive surface.
*
* This class is tought to be used by the
* main (OpenGL) thread only because it uses
* OpenGl methods in order to draw the
* graphics of each fiducial.
*/

#ifndef FIDUCIALS_MANAGER_H_
#define FIDUCIALS_MANAGER_H_

#include "ofMain.h"
#include "SettingsManager.h"
#include "TimingThread.h"
#include "OscManager.h"
#include "Sequencer.h"
#include "FiducialBase.h"
#include "FiducialPercussion.h"
#include "FiducialMetro.h"
#include "FiducialBass.h"
#include "FiducialPad.h"
#include "FiducialChord.h"
#include "udacn.h"

class FiducialsManager
{
private:
  // surface dimensions
  int width;
  int height;

  // a reference to the timing thread
  // in order to get some value from it
  TimingThread *thread;

  // We need a reference to the main sequencer
  // in order to add new instruments to it
  // and remove them when needed.
  Sequencer *seq;

  // we need a reference to the OSC Manager
  // in order to get information about the
  // sound level coming from PureData
  OscManager *oscMan;

  // a variable to store the current column
  // played by the sequencer
  int index, previous_index;

  // keep track of the fiducials on the surface
  vector< FiducialBase* > fiducialVector;

  // keep a reference to the app settings
  struct SettingsManager::settings *s;


  // below are variables to store data we get from
  // the incoming OSC communication
  float level;

  // We use a single udacn object to keep track
  // of all the movements done by alle the
  // 'metro' fiducials.
  // This object will communicate to the timing
  // thread, and will share with it the 'bpm' value.
  Udacn udacn;

  // A variable to keep track of the bpm value.
  // This variable is updated - and the
  // same is done for the timing thread -
  // every time the metro 'tuioAdd' or 'tuioUpdate'
  // callback methods are called
  int bpm;
  int minBpm, maxBpm;


  // Below are some variables to keep track of 'metro'
  // fiducials manipulation.
  //
  // 'angleRange': represent how much we must rotate
  // each 'metro' fiducial to get the complete bpm excursion.
  // We get this information from the 'metro' fiducial
  // settings we find inside the XML file.
  //
  // 'angleGap': represent how wide the graphical
  // representation of this excursion will be.
  // There's no need at all that the two values of
  // `angleRange` and `angleGap` are equals.
  //
  // They are both expressed in radians. Keep an eye
  // when transferring them to the 'udacn' tool and
  // to the 'metro' fiducials.
  float angleRange;
  float angleGap;

  // variables to take note of used 'fid's
  // TODO: rename this variable to FID_XXXX to ID_XXXX (?)
  int FID_KICK;
  int FID_SNARE;
  int FID_HIHAT;
  int FID_METRO;
  int FID_BASS;
  int FID_PAD;
  int FID_CHORD_1;
  int FID_CHORD_2;
  int FID_CHORD_3;
  int FID_CHORD_4;
  int FID_CHORD_5;
  int FID_CHORD_6;

public:
  FiducialsManager();
  ~FiducialsManager();

  void setup(int _w, int _h, struct SettingsManager::settings *_s, TimingThread *_t, Sequencer *_seq, OscManager *_oscMan);
  void update(int _index );
  void draw();

  // methods for adding, updating and removing fiducials
  // these methods are also used to add new elements
  // to the main sequencer (and also to remove them).
  // These methods are calleb by 'TUIO Manager'
  void tuioAdd( long sid, int fid, float x, float y, float angle, float rspeed );
  void tuioUpdate( long sid, int fid, float x, float y, float angle, float rspeed );
  void tuioRemove( long sid, int fid );

  void saveSettings();
  void loadSettings();

  // These methods are called by the surface manager
  // in order to obtain some information to show
  // on screen for debug purposes
  int getFiducialsNumber();
  int getBpm();

  // this are alterante methods to add and remove fiducials
  // in order to have another way to debug the application
  // bool inside( float x, float y );
  // int getSidFromPos( float x, float y);
};

#endif /* FIDUCIALS_MANAGER_H_ */
