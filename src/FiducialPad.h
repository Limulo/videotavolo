#ifndef FIDUCIALS_PAD_H_
#define FIDUCIALS_PAD_H_

#include "FiducialBase.h"
#include "Animator.h"
#include "ofMain.h"

class FiducialPad: public FiducialBase
{
protected:
  int r, g, b;
  AnimatorSine anim;
  float y_anim;

public:
  FiducialPad(int fid, int sid, struct SettingsManager::settings *_s);
  ~FiducialPad();

  //void setup();
  void update();
  void draw();

  void tuioAdd( float x, float y, float angle, float rspeed );
  void tuioUpdate( float x, float y, float angle, float rspeed );
  void tuioRemove();

  void saveSettings();
  void loadSettings();

  AnimatorSine* getAnimator() { return &anim; }
};
#endif /* FIDUCIALS_PAD_H_ */
