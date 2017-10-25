#ifndef OSC_MANAGER_H_
#define OSC_MANAGER_H_

#include "ofxOsc.h"
#include "SettingsManager.h"

class OscManager
{
private:

  ofxOscSender sender;
  ofxOscReceiver receiver;

  bool STATUS; //on-off
  string HOST;
  int SEND_PORT;
  int LISTEN_PORT;

  // keep a reference to the app settings
  struct SettingsManager::settings *s;

public:

  OscManager();
  ~OscManager();

  // methods below are called from the main thread
  void setup( SettingsManager::settings *s );
  void update(); // a.k.a receive

  // a function to interactively turn
  // OSC processing "on" or "off"
  // -- used by the main thread
  void swap();

  // method below are called from the sequencer object
  //void send(string s, int & i);
  void send(string s, int i);

  // below some data we used temporarly
  // for the main thread to get the level value
  float level;
  float getLevel(); // called by Fiducial Manager

  // function to save & load useful setting for
  // the class/subclass.
  // -- used by the main thread
  void saveSettings();
  void loadSettings();

};

#endif /* OSC_MANAGER_H_ */
