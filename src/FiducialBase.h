/*
* Fiducial Base is an abstract class used
* as the base for all the other fiducials classes
*/

#ifndef FIDUCIALS_BASE_H_
#define FIDUCIALS_BASE_H_

#include "ofMain.h"
#include "SettingsManager.h"
#include "Animator.h"

class FiducialBase
{
protected:

  int sid; // session ID
  int fid; // fiducial ID

  // here we store position data from TUIO (float32, 0-1)
  ofVec2f	pos;
  // here we store the rotation angle from TUIO (float32, 0-2PI)
  float	angle;
  // here we store the rotation speed from TUIO
  float rspeed;

  /* GRAPHICS ***********************************************/
  AnimatorASR asr;
  float y_asr;
  // the value below are pseudo-const velues;
  // them are ment to be changed only sometimes
  // when settings are loaded from the XML
  int FADEIN_TIME;
  int FADEOUT_TIME;

  // TODO: should SIZE be a static paramenter
  // (in order to have it once for all the fiducials)?
  int SIZE;

  // the variables below are used to create fiducial animations
  float size;
  float alpha;

  // this boolean value is used to take track of
  // fiducial graphical rapresentation even if
  // their corresponging tangible object on the
  // interactive surface has been removed and the
  // corresponding sequencer counterpart has been
  // removed too.
  // Indeed animation must go on, with a gentle fade out,
  // when the fiducial is removed from the table and then,
  // and only then, removed from the vector.
  bool alive;

  // keep a reference to the app settings
  struct SettingsManager::settings *s;

public:
  FiducialBase(int fid, int sid, struct SettingsManager::settings *_s);
  virtual ~FiducialBase();

  int getSid() { return sid; }
  int getFid() { return fid; }
  bool isAlive() { return alive; }

  // to ensure that each of the derived classes
  // will implement these methods, we make them
  // pure virtual.

  //virtual void setup() = 0;
  virtual void update() = 0;
  virtual void draw() = 0;
  void drawDebug( string name );

  //virtual void tuioAdd( float x, float y, int posInBar ) = 0; // will be like the "setup" method
  //virtual void tuioUpdate( float x, float y, int posInBar ) = 0;
  virtual void tuioAdd( float x, float y, float angle, float rspeed ) = 0; // will be like the "setup" method
  virtual void tuioUpdate( float x, float y, float angle, float rspeed ) = 0;
  virtual void tuioRemove() = 0;


  // TODO: is the following methods necessary? fiducials other than
  // percussion fids don't need to know their "position in bar"
  //virtual void tuioAdd( float x, float y );
  //virtual void tuioUpdate( float x, float y );


  // function to save & load useful setting for
  // the class/subclass.
  // TODO: not every class may need to save or
  // load settings. How to write the following
  // functions in order to do that??
  void saveSettings();
  void loadSettings();


  //virtual int getPosInBar() = 0;
  // TODO: try to delete this function in Base
  // and see if code still compiles
  //virtual void trigger() = 0;



  // this are alterante methods to add and remove fiducials
  // in order to have another way to debug the application
  //bool inside( float x, float y);
};

#endif /* FIDUCIALS_BASE_H_ */
