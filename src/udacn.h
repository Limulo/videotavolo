/**
 * U.D.A.C.N.
 * (unwrap, differentiate, accumulate,
 * constrain and normalize)
 *
 * This is a class built in order to manage rotational
 * data coming from a fiducial manipulator.
 * This data consist of a 'angle' and a
 * 'rotational speed' values.
 * We must use them to calculate a differential
 * accumulator and constrain inside a given range
 * and normalize it in order to get and output
 * between 0.0 and 1.0, ready to be used to drive
 * some other parameter.
 *
 * In other words this class is useful
 * if you want to use a fiducial
 * and its rotational properties to drive
 * any kind of fixed range parameter.
 */

#ifndef UDACN_H_
#define UDACN_H_

#include "ofMain.h"


class Udacn
{
private:
  float current, previous; // used in unwrapping calculations
  float diff, accum;

  // in order to filter out some decimal imprecision
  // between consecutive reading among angle unwrapping,
  // we use some sort of threshold mechanism.
  float filter = 0.001;

  float normalized; // normalized cursor
  float inRange;

public:

  Udacn();
  ~Udacn();

  void setup( float inRange, float initialValue );
  void bind( float angle );
  void update( float angle, float rotationSpeed );
  float getOutput();
};

#endif /* UDACN_H_ */
