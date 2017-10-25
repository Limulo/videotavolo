/*
* The sequencer is responsible of mantaining the table:
* a data structure where to save every `sid` and `instrument type`
* associated with every fiducials used on the interactive surface.
*/

#ifndef SEQUENCER_H_
#define SEQUENCER_H_

#include "OscManager.h"

#define PLANES 4
#define COLS 8
#define ROWS 4

class Sequencer
{
private:
  int table[ROWS][COLS][PLANES];
  OscManager * oscMan;

public:

  Sequencer();
  ~Sequencer();

  // methods below are called by the main thread
  void setup(OscManager * _oscMan );

  // methods below are called by event generated
  // and managed by the TUIO Manager
  void tuioAdd(int plane, int column, int sess_id);
  void tuioUpdate(int plane, int column, int sess_id);
  void tuioRemove(int sess_id);
  void tuioRemove(int plane, int sess_id);

  // method below is a callback for the timing thread
  void play( int & column );

  // methods below
  int	 get(int r, int c, int p){ return table[r][c][p]; }

  void print();
  void print(int plane);
};

#endif /* SEQUENCER_H_ */
