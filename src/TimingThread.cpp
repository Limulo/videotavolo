#include "TimingThread.h"
//using namespace std;

void TimingThread::start( struct SettingsManager::settings *_s )
{
  ofLogVerbose(" time thr. ") << "start - " << getThreadId() << " (name: " << getThreadName() << ").";

  // get the reference to the sittings
  s = _s;

  startTime_64n = ofGetElapsedTimeMillis();

  // we use this temporary variable
  // in order to send the initial notification
  index_1n  = previous_index_1n  = 0;
  index_2n  = previous_index_2n  = 0;
  index_4n  = previous_index_4n  = 0;
  index_8n  = previous_index_8n  = 0;
  index_16n = previous_index_16n = 0;
  index_32n = previous_index_32n = 0;
  index_64n = previous_index_64n = 0;
  ofNotifyEvent( ev, index_8n );

  pos = 0.0;

  loadSettings();

  startThread();
}


void TimingThread::threadedFunction()
{
  while( isThreadRunning() )
  {
    lock();
    // calculate the bar duration
    dur_1n = 60 * 1000 * 4 / bpm;
    unlock();
    // calculate other notes duration
    dur_2n  = dur_1n  * 0.5;
    dur_4n  = dur_2n  * 0.5;
    dur_8n  = dur_4n  * 0.5;
    dur_16n = dur_8n  * 0.5;
    dur_32n = dur_16n * 0.5;
    dur_64n = dur_32n * 0.5;

    // calculate the elapsed time, one for all ***************************
    elapsedTime_64n = ofGetElapsedTimeMillis();

    // time calculation - and notificaition - for 8n *********************
    deltaTime_64n = elapsedTime_64n - startTime_64n;
    if( deltaTime_64n >= dur_64n)
    {
      index_64n = (index_64n + 1) % 64;
      index_32n = index_64n * 0.5;
      index_16n = index_32n * 0.5;
      lock();
      index_8n = index_16n * 0.5;
      unlock();
      index_4n = index_8n * 0.5;
      index_2n = index_4n * 0.5;
      index_1n = index_2n * 0.5;

      if(index_8n != previous_index_8n)
      {
        ofNotifyEvent( ev, index_8n );
        lock();
        previous_index_8n = index_8n;
        unlock();
      }

      // update the start time to be the time when the new 8n began
      startTime_64n = elapsedTime_64n - deltaTime_64n % dur_64n;
    }

    lock();
    pos = (index_64n / 64.0 ) + (1.0 * deltaTime_64n) / dur_1n; // * dur_1n + deltaTime_64n * dur_64n;
    unlock();

    // time to take a rest ***********************************************
    // TODO: do we really want to sleep?
    // Could this cause some problem?
    // Maybe better to use some different method in order
    // not to use the CPU while doing nothing.
    sleep(1);
  }
  ofLogVerbose(" time thr. ") << "stop - " << getThreadId() << ";";
}


void TimingThread::stop()
{
  //stopThread();
  waitForThread();
}


// not used for the moment
void TimingThread::saveSettings()
{
  ofLogVerbose(" time thr. ") << "save settings - " << getThreadId() << ";";
  // do nothing
}


void TimingThread::loadSettings()
{
  ofLogVerbose(" time thr. ") << "load settings - " << getThreadId() << ";";
  lock(); // don't think it is necessary
  bpm = int(s->bpm.value);
  unlock(); // don't think it is necessary
}
