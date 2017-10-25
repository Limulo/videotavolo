#include "udacn.h"

Udacn::Udacn() {
  ofLogVerbose("   udacn   ") << "constructed";
}

Udacn::~Udacn() {
  ofLogVerbose("   udacn   ") << "destroyed";
}

void Udacn::setup( float _inRange, float _initialValue )
{
  ofLogVerbose("   udacn   ") << "inRange: "<< _inRange << ", initial value: "<< _initialValue<<"setup";
  inRange = _inRange;
  current = previous = diff = 0.0;

  //accum = 0.0;
  //normalized = 0.0;

  accum = _initialValue; // a valuie between 0.0 and inRange;
  normalized = accum / inRange;
}


void Udacn::update( float _current, float _aSpeed )
{
  //ofLogVerbose("   udacn   ") << "(" <<_current<< ", " <<_aSpeed<< ") update";
  current = _current;
  diff = current - previous;

  /** Phase Unwarpping *******************************/
  if( _aSpeed > 0 )
  {
    // clockwise rotation
    if( diff < -filter )
    {
      // we are over an angle discontiuity
      current += 2*PI;
    }
  }
  else if( _aSpeed < 0 )
  {
    // counterclockwise rotation
    if( diff > filter )
    {
      // we are over an angle discontiuity
      current -= 2*PI;
    }
  }
  else
  {
    // no rotation at all, do nothing
    return ;
  }

  /** Angle Accumulation *****************************/
  diff = current - previous;
  // here we always add 2PI to deal with negative 'aCurrent' angles
  previous = fmod( current+2*PI, 2*PI );
  accum += diff;

  accum = ofClamp(accum, 0.0, inRange);

  /** Normalization **********************************/
  normalized = accum / inRange;
}

float Udacn::getOutput()
{
  //ofLogVerbose("   udacn   ") << "get output";
  return normalized;
}

// This function is called when a new 'manipulator'
// wants to take control over the parameter.
void Udacn::bind( float _angle )
{
  ofLogVerbose("   udacn   ") << "angle: " <<_angle<< ") bind";
  previous = _angle;
}
