#ifndef FIDUCIALS_PERCUSSION_H_
#define FIDUCIALS_PERCUSSION_H_

#include "FiducialBase.h"
#include "ofMain.h"


// properties to be loaded from XML
// when needed
// #define ATTACK_TIME_2 50
// #define RELEASE_TIME_2 250


class FiducialPercussion : public FiducialBase
{
protected:

  int TRIGGERIN_TIME;
  int TRIGGEROUT_TIME;

  // the position of the fiducial inside the bar
  int posInBar;

  AnimatorAR ar;
  float y_ar;

  // moved inside the Fiducial Base class
  //AnimatorASR asr;
  //float y_asr;

public:
  FiducialPercussion(int fid, int sid, struct SettingsManager::settings *_s);
  ~FiducialPercussion();

  //void setup();
  void update();
  virtual void draw() = 0;

  //void tuioAdd( float x, float y, int posInBar ); // will be like the "setup" method
  //void tuioUpdate( float x, float y, int posInBar );
  void tuioAdd( float x, float y, float angle, float rspeed ); // will be like the "setup" method
  void tuioUpdate( float x, float y, float angle, float rspeed );
  void tuioRemove();

  void setPosInBar( int _posInBar );
  int getPosInBar();
  void trigger();

  void saveSettings();
  void loadSettings();
};



class Kick : public FiducialPercussion
{
private:
    int r, g, b;
public:
  Kick(int fid, int sid, struct SettingsManager::settings *_s);
  ~Kick();
  //void update();
  void draw();
  void saveSettings();
  void loadSettings();
};



class Snare : public FiducialPercussion
{
private:
    int r, g, b;
public:
  Snare(int fid, int sid, struct SettingsManager::settings *_s);
  ~Snare();
  //void update();
  void draw();
  void saveSettings();
  void loadSettings();
};




class Hihat : public FiducialPercussion
{
private:
    int r, g, b;
public:
  Hihat(int fid, int sid, struct SettingsManager::settings *_s);
  ~Hihat();
  //void update();
  void draw();
  void saveSettings();
  void loadSettings();
};


#endif /* FIDUCIALS_PERCUSSION_H_ */
