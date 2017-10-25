/*
* This class is thread used to keep the timing stuff separated from
* the main app thread. So the timing can be more accurate!
*
* The timing is done by calculating each musical interval
* duration in relation with the BPM value sent from the main thread.
*
* Then we use the 64note duration as a base to calculate when all other
* musical accents happen.
*
* From the moment we want to know when there's a new 8 note, we
* send notification only when there's a change in ths particular index.
*
* The Timing thread is responsible to autonomously get the
* default BPM value from the settings.
*/

#ifndef TIME_THREAD_H_
#define TIME_THREAD_H_

#include "ofThread.h"
#include "ofUtils.h"
#include "ofEvents.h"
#include "SettingsManager.h"

class TimingThread: public ofThread
{
private:
  uint64_t dur_1n; // bar duration (ms)
  uint64_t dur_2n;
  uint64_t dur_4n;
  uint64_t dur_8n;
  uint64_t dur_16n;
  uint64_t dur_32n;
  uint64_t dur_64n;

  uint64_t startTime_64n;
  uint64_t elapsedTime_64n;
  uint64_t deltaTime_64n;

  // these variables are used to make a comparison with
  // the actual index in order to send a notification
  // when the two values are different.
  int previous_index_1n;
  int previous_index_2n;
  int previous_index_4n;
  int previous_index_8n;
  int previous_index_16n;
  int previous_index_32n;
  int previous_index_64n;

  // a reference to the settings
  struct SettingsManager::settings *s;

public:

  void start( struct SettingsManager::settings *_s );
  void stop();
  void threadedFunction();

  void saveSettings();
  void loadSettings();

  // below we have shared data **********************************************
  ofEvent<int> ev;

  uint16_t bpm; // the BPM of the song

  // these variables to keep track of of each musical
  // interval relative to the bar inside.
  int index_1n;
  int index_2n;
  int index_4n;
  int index_8n; // the only index value actually shared
  int index_16n;
  int index_32n;
  int index_64n;

  // the current (normalized) position
  // of the playhead inside the bar
  float pos;
};

#endif /* TIME_THREAD_H_ */
