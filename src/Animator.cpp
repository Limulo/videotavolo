#include "Animator.h"
//using namespace std;


 /************************************************************************/
 /*                                AR                                    */
 /************************************************************************/

AnimatorAR::AnimatorAR() { ofLogVerbose(" animator  ")<<" AR: constructed!";}
AnimatorAR::~AnimatorAR(){ ofLogVerbose(" animator  ")<<" AR: destroyed";}

void AnimatorAR::setup(long _t1, long _t2 ) {
  t1 = _t1;
  t2 = _t2;
  dt = st = y = 0;
  state = INVALID;
  ofLogVerbose(" animator  ")<<" AR: setup ( " << t1 << ", " << t2 << " )";
}

// this function returns the animator state
// which is a negative integer in case
// of an invalid state
int AnimatorAR::update()
{
  if( state == ATTACK ) {
    dt = ofGetElapsedTimeMillis() - st;
    y = (1.0*dt)/t1;
    if( y > 1.0 ) {
      y = 1.0;
      st = ofGetElapsedTimeMillis();
      state = RELEASE;
    }
  } else if ( state == RELEASE ) {
    dt = ofGetElapsedTimeMillis() - st;
    y = 1.0 - (1.0*dt/t2);
    if( y < 0.0 ) {
      y = 0.0;
      state = INVALID;
    }
  }
  return state;
}

void AnimatorAR::trigger() {
  st = ofGetElapsedTimeMillis();
  state = ATTACK;
}

float AnimatorAR::getY() {
  if( y < 0.0 )
    return 0.0;
  else if( y > 1.0 )
    return 1.0;
  else
    return y;
 }

 /************************************************************************/
 /*                              ASR                                     */
 /************************************************************************/

AnimatorASR::AnimatorASR() {ofLogVerbose(" animator  ")<<"ASR: constructed!";}
AnimatorASR::~AnimatorASR(){ofLogVerbose(" animator  ")<<"ASR: destroyed";}

void AnimatorASR::setup(long _t1, long _t2 ) {
  t1 = _t1;
  t2 = _t2;
  dt = st = y = 0;
  state = INVALID;
  ofLogVerbose(" animator  ")<<"ASR: setup ( " << t1 << ", " << t2 << " )";
}

// this functoin return the animator state
// which is a negative integer in case
// of an invalid state
int AnimatorASR::update()
{
  if(state == ATTACK) {
    dt = ofGetElapsedTimeMillis() - st;
    y =  (1.0*dt)/t1;
    if(y > 1.0) {
      y = 1.0;
      state = SUSTAIN;
    }
  } else if( state == RELEASE ) {
    dt = ofGetElapsedTimeMillis() - st;
    y =  1.0 - ( (1.0*dt)/t2 );
    if(y < 0.0) {
      y = 0.0;
      state = INVALID;
    }
  }
  return state;
}

void AnimatorASR::triggerAttack() {
  state = ATTACK;
  st = ofGetElapsedTimeMillis();
}

void AnimatorASR::triggerRelease() {
  if(state == SUSTAIN)
  {
    st = ofGetElapsedTimeMillis();
  }
  else if (state == ATTACK)
  {
    //st = int( ofGetElapsedTimeMillis()*(1+((1.0*t2)/t1)) - t2*(1 + ((1.0*st)/t1)) );
    st = ofGetElapsedTimeMillis()*(1+((1.0*t2)/t1)) - t2*(1 + ((1.0*st)/t1));
  }
  state = RELEASE;
}

float AnimatorASR::getY() {
  if( y < 0.0 )
    return 0.0;
  else if( y > 1.0 )
    return 1.0;
  else
    return y;
 }

/************************************************************************/
/*                              SINE                                    */
/************************************************************************/

AnimatorSine::AnimatorSine(){ ofLogVerbose(" animator  ")<<"SINE: constructed";}
AnimatorSine::~AnimatorSine(){ofLogVerbose(" animator  ")<<"SINE: destroyed";}

void AnimatorSine::setup( float _freq, float _phase )
{
  freq = _freq>0.0?_freq:1.0;
  phase = fmod( abs(_phase), 2*PI );
  t = t0 = ofGetElapsedTimeMillis() * 0.001;
  dt = t - t0;
  y = 0.0;
  ofLogVerbose(" animator  ")<<"SINE: setup ( freq: " << freq << ", phase: " << phase << " )";
}

void AnimatorSine::update()
{
  t = ofGetElapsedTimeMillis() * 0.001;
  dt = t - t0;
  y = sin( phase + 2*PI*freq*dt );
  y = (y+1.0)*0.5;
}

void AnimatorSine::changeFreq( float _freq )
{
  t = ofGetElapsedTimeMillis() * 0.001;
  dt = t - t0;
  phase = phase + 2*PI*freq*dt;
  phase = fmod(phase, 2*PI );
  t0 = t;
  freq = _freq;
}

float AnimatorSine::getY() {
  return y;
}


/************************************************************************/
/*                              RAMP                                    */
/************************************************************************/

AnimatorRamp::AnimatorRamp(){ ofLogVerbose(" animator  ")<<"RAMP: constructed";}
AnimatorRamp::~AnimatorRamp(){ofLogVerbose(" animator  ")<<"RAMP: destroyed";}

void AnimatorRamp::setup( float _freq, float _phase, bool _inverse )
{
  freq = _freq>0.0?_freq:1;
  phase = fmod( abs(_phase), 2*PI );
  // At the beginning 't0' and 't' are equal.
  t = t0 = ofGetElapsedTimeMillis() * 0.001;
  dt = t - t0;
  y = 0.0;
  // Set this boolean if you want the ramp
  // going the opposite direction.
  inverse = _inverse;
  ofLogVerbose(" animator  ")<<"RAMP: setup ( freq: " << freq << ", phase: " << phase << ", inverse: "<< inverse <<")";
}

void AnimatorRamp::update()
{
  t = ofGetElapsedTimeMillis() * 0.001;
  dt = t - t0;
  // Support variable to convert
  // the 0-2PI phase to a 0-1 one.
  float phaseT = phase/(2*PI);
  y = fmod( (phaseT + dt*freq), 1.0);
  if( inverse )
    y = 1.0 - y;
}

void AnimatorRamp::changeFreq( float _freq )
{
  // calculate the phase for the
  // upcoming sinusoid
  t = ofGetElapsedTimeMillis() * 0.001;
  dt = t -t0;
  phase = phase + 2*PI*freq*dt;
  phase = fmod( phase, 2*PI );
  // define the new 't0' which is the
  // new reference for counting time
  t0 = t;
  // Finally, set the new frequency
  freq = _freq;
}

float AnimatorRamp::getY() {
  return y;
}

/************************************************************************/
/*                              RECT                                    */
/************************************************************************/

AnimatorRect::AnimatorRect(){ ofLogVerbose(" animator  ")<<"RECT: constructed";}
AnimatorRect::~AnimatorRect(){ofLogVerbose(" animator  ")<<"RECT: destroyed";}

void AnimatorRect::setup( float _freq, float _phase, float _duty, bool _inverse )
{
  freq = _freq>0.0?_freq:1;
  phase = fmod( _phase, 2*PI );
  // At the beginning 't0' and 't' are equal.
  t = t0 = ofGetElapsedTimeMillis() * 0.001;
  dt = t - t0;
  ctrl = y = 0.0;
  changeDutyCycle( _duty );
  ofLogVerbose(" animator  ")<<"RAMP: setup ( freq: " << freq <<
                                ", phase: " << phase <<
                                ", duty: " << duty <<
                                ", inverse: "<< inverse <<
                                ")";

}

void AnimatorRect::update()
{
  t = ofGetElapsedTimeMillis() * 0.001;
  dt = t - t0;
  // Support variable to convert
  // the 0-2PI phase to a 0-1 one.
  float phaseT = phase/(2*PI);

  ctrl = fmod( (phaseT+freq*dt), 1.0);

  if( ctrl <= duty )
    y = 1.0;
  else
    y = 0.0;
}

void AnimatorRect::changeFreq( float _freq )
{
  // calculate the phase for the
  // upcoming sinusoid
  t = ofGetElapsedTimeMillis() * 0.001;
  dt = t - t0;
  phase = phase + 2*PI*freq*dt;
  phase = fmod( phase, 2*PI );
  // define the new 't0' which is the
  // new reference for counting time
  t0 = t;
  // Finally, set the new frequency
  freq = _freq;
}

void AnimatorRect::changeDutyCycle( float _duty )
{
  duty = _duty;
  if( duty > 1.0 )
    duty = 1.0;
  else if( duty < 0.0 )
    duty = 0.0;
}

float AnimatorRect::getY() {
  return y;
}
