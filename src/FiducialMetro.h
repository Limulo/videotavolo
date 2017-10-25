#ifndef FIDUCIALS_METRO_H_
#define FIDUCIALS_METRO_H_

#include "FiducialBase.h"
#include "ofMain.h"
#include "udacn.h"

class Meter;

class FiducialMetro: public FiducialBase
{
protected:
  int r, g, b;

  ofVec2f playgroundMiddle;

  Udacn* udacn;
  float angleGap;
  Meter* meter;

  // support variables to calculate
	// the correct orientation of the
	// meter in respect the playground center.
	ofVec2f link;
	float beta; // degrees

public:
  FiducialMetro(int fid, int sid, struct SettingsManager::settings *_s);
  FiducialMetro(int fid, int sid, struct SettingsManager::settings *_s,
                Udacn* udacn, float playgroundWidth, float playgroundHeight,
                float angleGap);
  ~FiducialMetro();

  //void setup();
  void update();
  void draw();

  void tuioAdd( float x, float y, float angle, float rspeed );
  void tuioUpdate( float x, float y, float angle, float rspeed );
  void tuioRemove();

  void saveSettings();
  void loadSettings();
};

/************************************************************************/
/*                              METER                                   */
/************************************************************************/

class Meter
{
private:
  // maintain a reference to the app settings
  struct SettingsManager::settings *s;

  // position of the metro fiducial
  ofVec2f pos;
  ofVec2f * playgroundMiddle;

  // geometry and graphics of the meter
	int resolution;
	float radius; // inner circle radius
	float w; 					// width of the meter
	float angleGap; // 'height' of the meter
	ofPath backPath, frontPath;

  // a variable to keep trackof the (normalized)
  //level to represent.
  float level;
  float y_asr;
  float alpha;

  ofColor backColor, frontColor;

  void calculateBackPath();
  void calculateFrontPath( float level );

public:
   Meter( struct SettingsManager::settings *_s, ofVec2f *_playgroundMiddle, float angleGap );
  ~Meter();

  void update( float y_asr);
  void draw();

  void tuioAdd( float x, float y, float level);
  void tuioUpdate( float x, float y, float level);
  //void tuioRemove();

  void saveSettings();
  void loadSettings();

};

#endif /* FIDUCIALS_METRO_H_ */
