/*
* Animator
*/

#ifndef ANIMATOR_H_
#define ANIMATOR_H_

#include "ofMain.h" // for ofGetElapsedTimeMillis()

/************************************************************************/
/*                                AR                                    */
/************************************************************************/

class AnimatorAR
{
private:
  long t1, t2;
  long dt;
  long st;
  float y;

  enum {
    INVALID = -1,
    ATTACK,
    RELEASE
  } state;

public:
  AnimatorAR();
  ~AnimatorAR();

  void setup(long _t1, long _t2 );
  int update();
  void trigger();
  float getY();
};

/************************************************************************/
/*                              ASR                                     */
/************************************************************************/

class AnimatorASR
{
private:
  long t1, t2;
  long dt;
  long st;
  float y;

  enum {
    INVALID = -1,
    ATTACK,
    SUSTAIN,
    RELEASE
  } state;

public:
  AnimatorASR();
  ~AnimatorASR();

  void setup(long _t1, long _t2 );
  int update();
  void triggerAttack();
  void triggerRelease();
  float getY();
};


/************************************************************************/
/*                              SINE                                    */
/************************************************************************/

class AnimatorSine
{
private:
  float t, t0, dt; // mind the fact here times are floats!
  float y;
  float freq, phase; // 0-2PI
public:
  AnimatorSine();
  ~AnimatorSine();

  void setup( float _freq=1.0, float _phase=0.0 );
  void update();
  void changeFreq( float _freq );
  float getY();
};

/************************************************************************/
/*                              RAMP                                    */
/************************************************************************/

class AnimatorRamp
{
private:
  float t, t0, dt;
  float y;
  float freq, phase;
  bool inverse;
public:
  AnimatorRamp();
  ~AnimatorRamp();

  void setup( float _freq=1.0, float _phase=0.0, bool _inverse=false );
  void update();
  void changeFreq( float _freq );
  float getY();
};

/************************************************************************/
/*                              RECT                                    */
/************************************************************************/

class AnimatorRect
{
private:
  float t, t0, dt;
  float ctrl, y;
  float freq, phase, duty;
  bool inverse;

public:
  AnimatorRect();
  ~AnimatorRect();

  void setup( float _freq=1.0, float _phase=0.0, float _duty=0.5, bool _inverse=false );
  void update();
  void changeFreq( float _freq );
  void changeDutyCycle( float _duty );
  float getY();
};

#endif /* ANIMATOR_H_*/
