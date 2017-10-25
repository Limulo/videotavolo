#ifndef FIDUCIALS_CHORD_H_
#define FIDUCIALS_CHORD_H_

#include "FiducialBase.h"
#include "ofMain.h"

class FiducialChord: public FiducialBase
{
protected:
  AnimatorRamp anim;
  float y_anim;
public:
  FiducialChord(int fid, int sid, struct SettingsManager::settings *_s);
  ~FiducialChord();

  //void setup();
  void update();
  virtual void draw() = 0;

  void tuioAdd( float x, float y, float angle, float rspeed );
  void tuioUpdate( float x, float y, float angle, float rspeed );
  void tuioRemove();

  void saveSettings();
  void loadSettings();

  AnimatorRamp* getAnimator() { return &anim; }
};


class Chord1 : public FiducialChord
{
private:
    int r, g, b;
public:
  Chord1(int fid, int sid, struct SettingsManager::settings *_s);
  ~Chord1();

  void draw();
  void saveSettings();
  void loadSettings();
};


class Chord2 : public FiducialChord
{
private:
    int r, g, b;
public:
  Chord2(int fid, int sid, struct SettingsManager::settings *_s);
  ~Chord2();

  void draw();
  void saveSettings();
  void loadSettings();
};


class Chord3 : public FiducialChord
{
private:
    int r, g, b;
public:
  Chord3(int fid, int sid, struct SettingsManager::settings *_s);
  ~Chord3();

  void draw();
  void saveSettings();
  void loadSettings();
};


class Chord4 : public FiducialChord
{
private:
    int r, g, b;
public:
  Chord4(int fid, int sid, struct SettingsManager::settings *_s);
  ~Chord4();

  void draw();
  void saveSettings();
  void loadSettings();
};


class Chord5 : public FiducialChord
{
private:
    int r, g, b;
public:
  Chord5(int fid, int sid, struct SettingsManager::settings *_s);
  ~Chord5();

  void draw();
  void saveSettings();
  void loadSettings();
};


class Chord6 : public FiducialChord
{
private:
    int r, g, b;
public:
  Chord6(int fid, int sid, struct SettingsManager::settings *_s);
  ~Chord6();

  void draw();
  void saveSettings();
  void loadSettings();
};

#endif /* FIDUCIALS_CHORD_H_ */
