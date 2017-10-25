#ifndef FIDUCIALS_BASS_H_
#define FIDUCIALS_BASS_H_

#include "FiducialBase.h"
#include "ofMain.h"

class FiducialBass: public FiducialBase
{
protected:
  int r, g, b;
  float level;

public:
  FiducialBass(int fid, int sid, struct SettingsManager::settings *_s);
  ~FiducialBass();

  //void setup();
  void update();
  void draw();

  void tuioAdd( float x, float y, float angle, float rspeed );
  void tuioUpdate( float x, float y, float angle, float rspeed);
  void tuioRemove();

  void saveSettings();
  void loadSettings();

  void setLevel( float level );
};
#endif /* FIDUCIALS_BASS_H_ */
