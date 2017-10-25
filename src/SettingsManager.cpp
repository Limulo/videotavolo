#include "SettingsManager.h"
//using namespace std;

SettingsManager::SettingsManager() {
  ofLogVerbose("  sett man ") << "constructed!";
}

SettingsManager::~SettingsManager() {
  ofLogVerbose("  sett man ") << "destroyed";
}

void SettingsManager::setup() {
  // this is redundant, custom setting
  // already contain default settings!
  // loadDefaults();
  loadSettings();
}

void SettingsManager::loadDefaults() {
  ofLogVerbose("  sett man ") << "load defaults";
  customs = defaults;
  printSettings();
}





void SettingsManager::loadSettings()
{
  ofLogVerbose("  sett man ") << "load settings ";

  XML.clear();

  // try to load the XML settings document
  if( !XML.load("settings.xml") )
  {
    ofLogError("  sett man ") << "\tLoading 'settings.xml' failed";
    return;
  }

  ofLogNotice("  sett man ") << "\tLoading 'settings.xml' succeeded!";

  // Custom settings already contains default values
  // because of the initialization of the structures
  // but, if we found a custom value/attribute
  // for the parameter inside the XML file, we use it.

  string s = "";
  /* BPM settings *************************************/
  s = "//bpm[@value]";
  if( XML.exists(s) )
    customs.bpm.value = ofToInt( XML.getAttribute(s) );

  s = "//bpm[@min]";
  if( XML.exists(s) )
    customs.bpm.min = ofToInt( XML.getAttribute(s) );

  s = "//bpm[@max]";
  if( XML.exists(s) )
    customs.bpm.max = ofToInt( XML.getAttribute(s) );

  /* OSC settings *************************************/
  s = "//OSC[@status]";
  if( XML.exists(s) )
    customs.osc.status = XML.getAttribute(s);

  s = "//OSC[@host]";
  if( XML.exists(s) )
    customs.osc.host = XML.getAttribute(s);

  s = "//OSC[@send]";
  if( XML.exists(s) )
    customs.osc.send = XML.getAttribute(s);

  s = "//OSC[@listen]";
  if( XML.exists(s) )
    customs.osc.listen = XML.getAttribute(s);

  /* TUIO settings ************************************/
  s = "//TUIO[@status]";
  if( XML.exists(s) )
    customs.tuio.status = XML.getAttribute(s);

  s = "//TUIO[@port]";
  if( XML.exists(s) )
    customs.tuio.listen = XML.getAttribute(s);

  /* Aspect settings **********************************/
  /* Background ***************************************/
  s = "//aspect/background[@r]";
  if( XML.exists(s) )
    customs.aspect.background.r = ofToInt( XML.getAttribute(s) );

  s = "//aspect/background[@g]";
  if( XML.exists(s) )
    customs.aspect.background.g = ofToInt( XML.getAttribute(s) );

  s = "//aspect/background[@b]";
  if( XML.exists(s) )
    customs.aspect.background.b = ofToInt( XML.getAttribute(s) );

  /* Fbo **********************************************/
  s = "//aspect/fbo[@r]";
  if( XML.exists(s) )
    customs.aspect.fbo.r = ofToInt( XML.getAttribute(s) );

  s = "//aspect/fbo[@g]";
  if( XML.exists(s) )
    customs.aspect.fbo.g = ofToInt( XML.getAttribute(s) );

  s = "//aspect/fbo[@b]";
  if( XML.exists(s) )
    customs.aspect.fbo.b = ofToInt( XML.getAttribute(s) );

  /* Playhead *****************************************/
  s = "//aspect/playhead[@r]";
  if( XML.exists(s) )
    customs.aspect.playhead.r = ofToInt( XML.getAttribute(s) );

  s = "//aspect/playhead[@g]";
  if( XML.exists(s) )
    customs.aspect.playhead.g = ofToInt( XML.getAttribute(s) );

  s = "//aspect/playhead[@b]";
  if( XML.exists(s) )
    customs.aspect.playhead.b = ofToInt( XML.getAttribute(s) );

  /* Distorion settings *******************************/
  s = "//aspect/distorsion[@moveX]";
  if( XML.exists(s) )
    customs.aspect.distorsion.moveX = ofToFloat( XML.getAttribute(s) );

  s = "//aspect/distorsion[@moveY]";
  if( XML.exists(s) )
    customs.aspect.distorsion.moveY = ofToFloat( XML.getAttribute(s) );

  s = "//aspect/distorsion[@rotX]";
  if( XML.exists(s) )
    customs.aspect.distorsion.rotX = ofToFloat( XML.getAttribute(s) );

  s = "//aspect/distorsion[@rotZ]";
  if( XML.exists(s) )
    customs.aspect.distorsion.rotZ = ofToFloat( XML.getAttribute(s) );

  s = "//aspect/distorsion[@scale]";
  if( XML.exists(s) )
    customs.aspect.distorsion.scale = ofToFloat( XML.getAttribute(s) );

  /* FIDUCIAL Generic settings ************************/
  s = "//fiducials[@size]";
  if( XML.exists(s) )
    customs.fiducials.size = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/fadetime[@in]";
  if( XML.exists(s) )
    customs.fiducials.fadetime.in = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/fadetime[@out]";
  if( XML.exists(s) )
    customs.fiducials.fadetime.out = ofToInt( XML.getAttribute(s) );

  /* FIDUCIAL PERCUSSION settings *********************/
  s = "//fiducials/percussions/triggertime[@in]";
  if( XML.exists(s) )
    customs.fiducials.percussions.triggertime.in = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/percussions/triggertime[@out]";
  if( XML.exists(s) )
    customs.fiducials.percussions.triggertime.out = ofToInt( XML.getAttribute(s) );

  /* FIDUCIAL PERCUSSION KICK settings ****************/
  s = "//fiducials/percussions/kick[@fid]";
  if( XML.exists(s) )
    customs.fiducials.percussions.kick.fid = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/percussions/kick/color[@b]";
  if( XML.exists(s) )
    customs.fiducials.percussions.kick.color.b = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/percussions/kick/color[@g]";
  if( XML.exists(s) )
    customs.fiducials.percussions.kick.color.g = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/percussions/kick/color[@r]";
  if( XML.exists(s) )
    customs.fiducials.percussions.kick.color.r = ofToInt( XML.getAttribute(s) );

  /* FIDUCIAL PERCUSSION SNARE settings ***************/
  s = "//fiducials/percussions/snare[@fid]";
  if( XML.exists(s) )
    customs.fiducials.percussions.snare.fid = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/percussions/snare/color[@b]";
  if( XML.exists(s) )
    customs.fiducials.percussions.snare.color.b = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/percussions/snare/color[@g]";
  if( XML.exists(s) )
    customs.fiducials.percussions.snare.color.g = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/percussions/snare/color[@r]";
  if( XML.exists(s) )
    customs.fiducials.percussions.snare.color.r = ofToInt( XML.getAttribute(s) );

  /* FIDUCIAL PERCUSSION HIHAT settings ***************/
  s = "//fiducials/percussions/hihat[@fid]";
  if( XML.exists(s) )
    customs.fiducials.percussions.hihat.fid = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/percussions/hihat/color[@b]";
  if( XML.exists(s) )
    customs.fiducials.percussions.hihat.color.b = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/percussions/hihat/color[@g]";
  if( XML.exists(s) )
    customs.fiducials.percussions.hihat.color.g = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/percussions/hihat/color[@r]";
  if( XML.exists(s) )
    customs.fiducials.percussions.hihat.color.r = ofToInt( XML.getAttribute(s) );

  /* FIDUCIAL METRO settings **************************/
  s = "//fiducials/metro[@fid]";
  if( XML.exists(s) )
    customs.fiducials.metro.fid = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/metro/color[@b]";
  if( XML.exists(s) )
    customs.fiducials.metro.color.b = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/metro/color[@g]";
  if( XML.exists(s) )
    customs.fiducials.metro.color.g = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/metro/color[@r]";
  if( XML.exists(s) )
    customs.fiducials.metro.color.r = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/metro/meter[@angleRange]";
  if( XML.exists(s) )
    customs.fiducials.metro.meter.angleRange = ofToFloat( XML.getAttribute(s) );

  s = "//fiducials/metro/meter[@angleGap]";
  if( XML.exists(s) )
    customs.fiducials.metro.meter.angleGap = ofToFloat( XML.getAttribute(s) );

  s = "//fiducials/metro/meter[@radius]";
  if( XML.exists(s) )
    customs.fiducials.metro.meter.radius = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/metro/meter[@w]";
  if( XML.exists(s) )
    customs.fiducials.metro.meter.w = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/metro/meter/backColor[@r]";
  if( XML.exists(s) )
    customs.fiducials.metro.meter.backColor.r = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/metro/meter/backColor[@g]";
  if( XML.exists(s) )
    customs.fiducials.metro.meter.backColor.g = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/metro/meter/backColor[@b]";
  if( XML.exists(s) )
    customs.fiducials.metro.meter.backColor.b = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/metro/meter/frontColor[@r]";
  if( XML.exists(s) )
    customs.fiducials.metro.meter.frontColor.r = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/metro/meter/frontColor[@g]";
  if( XML.exists(s) )
    customs.fiducials.metro.meter.frontColor.g = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/metro/meter/frontColor[@b]";
  if( XML.exists(s) )
    customs.fiducials.metro.meter.frontColor.b = ofToInt( XML.getAttribute(s) );


  /* FIDUCIAL BASS settings ***************************/
  s = "//fiducials/bass[@fid]";
  if( XML.exists(s) )
    customs.fiducials.bass.fid = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/bass/color[@b]";
  if( XML.exists(s) )
    customs.fiducials.bass.color.b = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/bass/color[@g]";
  if( XML.exists(s) )
    customs.fiducials.bass.color.g = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/bass/color[@r]";
  if( XML.exists(s) )
    customs.fiducials.bass.color.r = ofToInt( XML.getAttribute(s) );

  /* FIDUCIAL PAD settings ****************************/
  s = "//fiducials/pad[@fid]";
  if( XML.exists(s) )
    customs.fiducials.pad.fid = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/pad/color[@b]";
  if( XML.exists(s) )
    customs.fiducials.pad.color.b = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/pad/color[@g]";
  if( XML.exists(s) )
    customs.fiducials.pad.color.g = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/pad/color[@r]";
  if( XML.exists(s) )
    customs.fiducials.pad.color.r = ofToInt( XML.getAttribute(s) );

  /* FIDUCIAL CHORDS settings *************************/
  /* CHORD 1 settings *********************************/
  s = "//fiducials/chords/chord1[@fid]";
  if( XML.exists(s) )
    customs.fiducials.chords.chord1.fid = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/chords/chord1/color[@b]";
  if( XML.exists(s) )
    customs.fiducials.chords.chord1.color.b = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/chords/chord1/color[@g]";
  if( XML.exists(s) )
    customs.fiducials.chords.chord1.color.g = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/chords/chord1/color[@r]";
  if( XML.exists(s) )
    customs.fiducials.chords.chord1.color.r = ofToInt( XML.getAttribute(s) );

  /* CHORD 2 settings *********************************/
  s = "//fiducials/chords/chord2[@fid]";
  if( XML.exists(s) )
    customs.fiducials.chords.chord2.fid = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/chords/chord2/color[@b]";
  if( XML.exists(s) )
    customs.fiducials.chords.chord2.color.b = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/chords/chord2/color[@g]";
  if( XML.exists(s) )
    customs.fiducials.chords.chord2.color.g = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/chords/chord2/color[@r]";
  if( XML.exists(s) )
    customs.fiducials.chords.chord2.color.r = ofToInt( XML.getAttribute(s) );

  /* CHORD 3 settings *********************************/
  s = "//fiducials/chords/chord3[@fid]";
  if( XML.exists(s) )
    customs.fiducials.chords.chord3.fid = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/chords/chord3/color[@b]";
  if( XML.exists(s) )
    customs.fiducials.chords.chord3.color.b = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/chords/chord3/color[@g]";
  if( XML.exists(s) )
    customs.fiducials.chords.chord3.color.g = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/chords/chord3/color[@r]";
  if( XML.exists(s) )
    customs.fiducials.chords.chord3.color.r = ofToInt( XML.getAttribute(s) );

  /* CHORD 4 settings *********************************/
  s = "//fiducials/chords/chord4[@fid]";
  if( XML.exists(s) )
    customs.fiducials.chords.chord4.fid = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/chords/chord4/color[@b]";
  if( XML.exists(s) )
    customs.fiducials.chords.chord4.color.b = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/chords/chord4/color[@g]";
  if( XML.exists(s) )
    customs.fiducials.chords.chord4.color.g = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/chords/chord4/color[@r]";
  if( XML.exists(s) )
    customs.fiducials.chords.chord4.color.r = ofToInt( XML.getAttribute(s) );

  /* CHORD 5 settings *********************************/
  s = "//fiducials/chords/chord5[@fid]";
  if( XML.exists(s) )
    customs.fiducials.chords.chord5.fid = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/chords/chord5/color[@b]";
  if( XML.exists(s) )
    customs.fiducials.chords.chord5.color.b = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/chords/chord5/color[@g]";
  if( XML.exists(s) )
    customs.fiducials.chords.chord5.color.g = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/chords/chord5/color[@r]";
  if( XML.exists(s) )
    customs.fiducials.chords.chord5.color.r = ofToInt( XML.getAttribute(s) );

  /* CHORD 6 settings *********************************/
  s = "//fiducials/chords/chord6[@fid]";
  if( XML.exists(s) )
    customs.fiducials.chords.chord6.fid = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/chords/chord6/color[@b]";
  if( XML.exists(s) )
    customs.fiducials.chords.chord6.color.b = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/chords/chord6/color[@g]";
  if( XML.exists(s) )
    customs.fiducials.chords.chord6.color.g = ofToInt( XML.getAttribute(s) );

  s = "//fiducials/chords/chord6/color[@r]";
  if( XML.exists(s) )
    customs.fiducials.chords.chord6.color.r = ofToInt( XML.getAttribute(s) );


  printSettings();

  // loose any reference to the XML file
  XML.clear();
}


/**
 * This function is used to save settings inside the XML file.
 * 1) check if the XML document already exhists;
 * 2) if so, use it and possibly update it with new attribute/values;
 * 3) if not, then create the new attribute e set the value for it
 */
void SettingsManager::saveSettings()
{
  ofLogVerbose("  sett man ") << "save settings ";

  XML.clear();

  // try to load the XML settings document
  if( !XML.load("settings.xml") )
  {
    ofLogError("  sett man ") << "\t'settings.xml' doesn't exist. I'll create a new one.";
    XML.addChild("settings");
  }
  else
  {
    ofLogNotice("  sett man ") << "\t'settings.xml' already exist. I'll use it!";
  }

  // Now go through all the settings and check
  // if the local XML handler contains them.
  // If not then create the new settings.

  /* BPM settings *************************************/
  XML.reset();
  if( !XML.setTo("bpm") )
  {
    //cout << "tag 'general' not found! I'll create a new one!" << endl;
    XML.addChild("bpm");
    XML.setTo("bpm");
  }
  XML.setAttribute("value", ofToString( customs.bpm.value ) );
  XML.setAttribute("min", ofToString( customs.bpm.min ) );
  XML.setAttribute("max", ofToString( customs.bpm.max ) );


  /* OSC settings *************************************/
  XML.reset();
  if( !XML.setTo("OSC") )
  {
    //cout << "tag 'OSC' not found! I'll create a new one!" << endl;
    XML.addChild("OSC");
    XML.setTo("OSC");
  }
  XML.setAttribute("host", ofToString( customs.osc.host ) );
  XML.setAttribute("listen", ofToString( customs.osc.listen ) );
  XML.setAttribute("send", ofToString( customs.osc.send ) );
  XML.setAttribute("status", ofToString( customs.osc.status ) );


  /* TUIO settings ************************************/
  XML.reset();
  if( !XML.setTo("TUIO") )
  {
    cout << "tag 'TUIO' not found! I'll create a new one!" << endl;
    XML.addChild("TUIO");
    XML.setTo("TUIO");
  }
  XML.setAttribute("listen", ofToString( customs.tuio.listen ) );
  XML.setAttribute("status", ofToString( customs.tuio.status) );

  XML.reset();
  /* Aspect settings **********************************/
  if( !XML.setTo("aspect") ) {
    XML.addChild("aspect");
    XML.setTo("aspect");
  }
  /* Background settings ******************************/
  if( !XML.setTo("background") )
  {
    cout << "tag 'background' not found! I'll create a new one!" << endl;
    XML.addChild("background");
    XML.setTo("background");
  }
  XML.setAttribute("r", ofToString( customs.aspect.background.r ) );
  XML.setAttribute("f", ofToString( customs.aspect.background.g ) );
  XML.setAttribute("b", ofToString( customs.aspect.background.b ) );
  XML.setToParent();

  /* Fbo settings *************************************/
  if( !XML.setTo("fbo") )
  {
    cout << "tag 'fbo' not found! I'll create a new one!" << endl;
    XML.addChild("fbo");
    XML.setTo("fbo");
  }
  XML.setAttribute("r", ofToString( customs.aspect.fbo.r ) );
  XML.setAttribute("f", ofToString( customs.aspect.fbo.g ) );
  XML.setAttribute("b", ofToString( customs.aspect.fbo.b ) );
  XML.setToParent();

  /* Playhead settings ********************************/
  if( !XML.setTo("playhead") )
  {
    cout << "tag 'playhead' not found! I'll create a new one!" << endl;
    XML.addChild("playhead");
    XML.setTo("playhead");
  }
  XML.setAttribute("r", ofToString( customs.aspect.playhead.r ) );
  XML.setAttribute("f", ofToString( customs.aspect.playhead.g ) );
  XML.setAttribute("b", ofToString( customs.aspect.playhead.b ) );
  XML.setToParent();

  /* Distorion settings *******************************/
  if( !XML.setTo("distorsion") )
  {
    cout << "tag 'distorsion' not found! I'll create a new one!" << endl;
    XML.addChild("distorsion");
    XML.setTo("distorsion");
  }
  XML.setAttribute("moveX", ofToString( customs.aspect.distorsion.moveX ) );
  XML.setAttribute("moveY", ofToString( customs.aspect.distorsion.moveY ) );
  XML.setAttribute("rotX",  ofToString( customs.aspect.distorsion.rotX  ) );
  XML.setAttribute("rotZ",  ofToString( customs.aspect.distorsion.rotZ  ) );
  XML.setAttribute("scale", ofToString( customs.aspect.distorsion.scale ) );

  /* FIDUCIAL Generic settings ************************/
  // XML.reset();
  // if( !XML.setTo("fiducials/fadetime") ) {
  //   //cout << "tag 'fiducials/fadetime' NOT found! I'll create a new one!" << endl;
  //   if( !XML.setTo("fiducials") ) {
  //     //cout << "tag 'fiducials' NOT found! I'll create a new one!" << endl;
  //     XML.addChild("fiducials");
  //     XML.setTo("fiducials");
  //   }
  //   XML.addChild("fadetime");
  //   XML.setTo("fadetime");
  // }
  // XML.setAttribute("in", ofToString( customs.fiducials.fadetime.in ) );
  // XML.setAttribute("out", ofToString( customs.fiducials.fadetime.out ) );


  XML.reset();
  if( !XML.setTo("fiducials") ) {
    //cout << "tag 'fiducials/fadetime' NOT found! I'll create a new one!" << endl;
    XML.addChild("fiducials");
    XML.setTo("fiducials");
  }
  XML.setAttribute("size", ofToString( customs.fiducials.size ) );

  if( !XML.setTo("fadetime") ) {
    //cout << "tag 'fiducials/fadetime' NOT found! I'll create a new one!" << endl;
    XML.addChild("fadetime");
    XML.setTo("fadetime");
  }

  XML.setAttribute("in", ofToString( customs.fiducials.fadetime.in ) );
  XML.setAttribute("out", ofToString( customs.fiducials.fadetime.out ) );
  XML.setToParent();


  /* FIDUCIAL Percussion settings *********************/
  if( !XML.setTo("percussions") ) {
    XML.addChild("percussions");
    XML.setTo("percussions");
  }
  //XML.setAttribute("size", ofToString( customs.fiducials.percussions.size ) );

  if( !XML.setTo("triggertime") ) {
    XML.addChild("triggertime");
    XML.setTo("triggertime");
  }
  XML.setAttribute("in", ofToString( customs.fiducials.percussions.triggertime.in ) );
  XML.setAttribute("out", ofToString( customs.fiducials.percussions.triggertime.out ) );
  XML.setToParent();

  /* KICK */
  if( !XML.setTo("kick") ) {
    XML.addChild("kick");
    XML.setTo("kick");
  }
  XML.setAttribute("fid", ofToString( customs.fiducials.percussions.kick.fid ) );

  if( !XML.setTo("color") ) {
    XML.addChild("color");
    XML.setTo("color");
  }
  XML.setAttribute("r", ofToString( customs.fiducials.percussions.kick.color.r ) );
  XML.setAttribute("g", ofToString( customs.fiducials.percussions.kick.color.g ) );
  XML.setAttribute("b", ofToString( customs.fiducials.percussions.kick.color.b ) );
  XML.setTo("../../");


  /* SNARE */
  if( !XML.setTo("snare") ) {
    XML.addChild("snare");
    XML.setTo("snare");
  }
  XML.setAttribute("fid", ofToString( customs.fiducials.percussions.snare.fid ) );

  if( !XML.setTo("color") ) {
    XML.addChild("color");
    XML.setTo("color");
  }
  XML.setAttribute("r", ofToString( customs.fiducials.percussions.snare.color.r ) );
  XML.setAttribute("g", ofToString( customs.fiducials.percussions.snare.color.g ) );
  XML.setAttribute("b", ofToString( customs.fiducials.percussions.snare.color.b ) );
  XML.setTo("../../");

  /* HIHAT */
  if( !XML.setTo("hihat") ) {
    XML.addChild("hihat");
    XML.setTo("hihat");
  }
  XML.setAttribute("fid", ofToString( customs.fiducials.percussions.hihat.fid ) );

  if( !XML.setTo("color") ) {
    XML.addChild("color");
    XML.setTo("color");
  }
  XML.setAttribute("r", ofToString( customs.fiducials.percussions.hihat.color.r ) );
  XML.setAttribute("g", ofToString( customs.fiducials.percussions.hihat.color.g ) );
  XML.setAttribute("b", ofToString( customs.fiducials.percussions.hihat.color.b ) );
  XML.setTo("../../"); // <-- we are inside 'percussion' tag


  XML.setToParent(); // <-- now we are inside 'fiducials' tag

  if( !XML.setTo("metro") ) {
    XML.addChild("metro");
    XML.setTo("metro");
  }
  XML.setAttribute("fid", ofToString( customs.fiducials.metro.fid ) );

  if( !XML.setTo("color") ) {
    XML.addChild("color");
    XML.setTo("color");
  }
  XML.setAttribute("r", ofToString( customs.fiducials.metro.color.r ) );
  XML.setAttribute("g", ofToString( customs.fiducials.metro.color.g ) );
  XML.setAttribute("b", ofToString( customs.fiducials.metro.color.b ) );
  XML.setToParent(); // <-- now we are inside 'metro' tag

  if( !XML.setTo("meter") ) {
    XML.addChild("meter");
    XML.setTo("meter");
  }
  XML.setAttribute("angleRange", ofToString( customs.fiducials.metro.meter.angleRange ) );
  XML.setAttribute("angleGap", ofToString( customs.fiducials.metro.meter.angleGap ) );
  XML.setAttribute("radius", ofToString( customs.fiducials.metro.meter.radius ) );
  XML.setAttribute("w", ofToString( customs.fiducials.metro.meter.w ) );

  if( !XML.setTo("backColor") ) {
    XML.addChild("backColor");
    XML.setTo("backColor");
  }
  XML.setAttribute("r", ofToString( customs.fiducials.metro.meter.backColor.r ) );
  XML.setAttribute("g", ofToString( customs.fiducials.metro.meter.backColor.g ) );
  XML.setAttribute("b", ofToString( customs.fiducials.metro.meter.backColor.b ) );
  XML.setToParent(); // <-- now we are inside 'meter' tag

  if( !XML.setTo("frontColor") ) {
    XML.addChild("frontColor");
    XML.setTo("frontColor");
  }
  XML.setAttribute("r", ofToString( customs.fiducials.metro.meter.frontColor.r ) );
  XML.setAttribute("g", ofToString( customs.fiducials.metro.meter.frontColor.g ) );
  XML.setAttribute("b", ofToString( customs.fiducials.metro.meter.frontColor.b ) );
  XML.setTo("../../"); // <-- we are inside 'metro' tag


  XML.setToParent(); // <-- now we are inside 'fiducials' tag


  if( !XML.setTo("bass") ) {
    XML.addChild("bass");
    XML.setTo("bass");
  }
  XML.setAttribute("fid", ofToString( customs.fiducials.bass.fid ) );

  if( !XML.setTo("color") ) {
    XML.addChild("color");
    XML.setTo("color");
  }
  XML.setAttribute("r", ofToString( customs.fiducials.bass.color.r ) );
  XML.setAttribute("g", ofToString( customs.fiducials.bass.color.g ) );
  XML.setAttribute("b", ofToString( customs.fiducials.bass.color.b ) );
  XML.setTo("../../"); // <-- we are inside 'fiducial' tag


  if( !XML.setTo("pad") ) {
    XML.addChild("pad");
    XML.setTo("pad");
  }
  XML.setAttribute("fid", ofToString( customs.fiducials.pad.fid ) );

  if( !XML.setTo("color") ) {
    XML.addChild("color");
    XML.setTo("color");
  }
  XML.setAttribute("r", ofToString( customs.fiducials.pad.color.r ) );
  XML.setAttribute("g", ofToString( customs.fiducials.pad.color.g ) );
  XML.setAttribute("b", ofToString( customs.fiducials.pad.color.b ) );
  XML.setTo("../../"); // <-- we are inside 'fiducial' tag


  if( !XML.setTo("chords") ) {
    XML.addChild("chords");
    XML.setTo("chords");
  }

  if( !XML.setTo("chord1") ) {
    XML.addChild("chord1");
    XML.setTo("chord1");
  }
  XML.setAttribute("fid", ofToString( customs.fiducials.chords.chord1.fid ) );

  if( !XML.setTo("color") ) {
    XML.addChild("color");
    XML.setTo("color");
  }
  XML.setAttribute("r", ofToString( customs.fiducials.chords.chord1.color.r ) );
  XML.setAttribute("g", ofToString( customs.fiducials.chords.chord1.color.g ) );
  XML.setAttribute("b", ofToString( customs.fiducials.chords.chord1.color.b ) );
  XML.setTo("../../"); // <-- we are inside 'chords' tag


  if( !XML.setTo("chord2") ) {
    XML.addChild("chord2");
    XML.setTo("chord2");
  }
  XML.setAttribute("fid", ofToString( customs.fiducials.chords.chord2.fid ) );

  if( !XML.setTo("color") ) {
    XML.addChild("color");
    XML.setTo("color");
  }
  XML.setAttribute("r", ofToString( customs.fiducials.chords.chord2.color.r ) );
  XML.setAttribute("g", ofToString( customs.fiducials.chords.chord2.color.g ) );
  XML.setAttribute("b", ofToString( customs.fiducials.chords.chord2.color.b ) );
  XML.setTo("../../"); // <-- we are inside 'chords' tag


  if( !XML.setTo("chord3") ) {
    XML.addChild("chord3");
    XML.setTo("chord3");
  }
  XML.setAttribute("fid", ofToString( customs.fiducials.chords.chord3.fid ) );

  if( !XML.setTo("color") ) {
    XML.addChild("color");
    XML.setTo("color");
  }
  XML.setAttribute("r", ofToString( customs.fiducials.chords.chord3.color.r ) );
  XML.setAttribute("g", ofToString( customs.fiducials.chords.chord3.color.g ) );
  XML.setAttribute("b", ofToString( customs.fiducials.chords.chord3.color.b ) );
  XML.setTo("../../"); // <-- we are inside 'chords' tag


  if( !XML.setTo("chord4") ) {
    XML.addChild("chord4");
    XML.setTo("chord4");
  }
  XML.setAttribute("fid", ofToString( customs.fiducials.chords.chord4.fid ) );

  if( !XML.setTo("color") ) {
    XML.addChild("color");
    XML.setTo("color");
  }
  XML.setAttribute("r", ofToString( customs.fiducials.chords.chord4.color.r ) );
  XML.setAttribute("g", ofToString( customs.fiducials.chords.chord4.color.g ) );
  XML.setAttribute("b", ofToString( customs.fiducials.chords.chord4.color.b ) );
  XML.setTo("../../"); // <-- we are inside 'chords' tag


  if( !XML.setTo("chord5") ) {
    XML.addChild("chord5");
    XML.setTo("chord5");
  }
  XML.setAttribute("fid", ofToString( customs.fiducials.chords.chord5.fid ) );

  if( !XML.setTo("color") ) {
    XML.addChild("color");
    XML.setTo("color");
  }
  XML.setAttribute("r", ofToString( customs.fiducials.chords.chord5.color.r ) );
  XML.setAttribute("g", ofToString( customs.fiducials.chords.chord5.color.g ) );
  XML.setAttribute("b", ofToString( customs.fiducials.chords.chord5.color.b ) );
  XML.setTo("../../"); // <-- we are inside 'chords' tag


  if( !XML.setTo("chord6") ) {
    XML.addChild("chord6");
    XML.setTo("chord6");
  }
  XML.setAttribute("fid", ofToString( customs.fiducials.chords.chord6.fid ) );

  if( !XML.setTo("color") ) {
    XML.addChild("color");
    XML.setTo("color");
  }
  XML.setAttribute("r", ofToString( customs.fiducials.chords.chord6.color.r ) );
  XML.setAttribute("g", ofToString( customs.fiducials.chords.chord6.color.g ) );
  XML.setAttribute("b", ofToString( customs.fiducials.chords.chord6.color.b ) );
  XML.setTo("../../"); // <-- we are inside 'chords' tag


  /* Save the file ************************************/
  if( XML.save("settings.xml") )
    ofLogNotice("  sett man ") << "\tSaving succeded!";
  else
    ofLogError("  sett man ") << "\tSaving failed.";

  printSettings();
}


/**
 * Print Settings
 */
void SettingsManager::printSettings()
{
  ofLogVerbose("  sett man ") << "print ";

  ofLogVerbose("  sett man ") << "\tBPM: value= " << customs.bpm.value << ", "
                              << "min= " << customs.bpm.min << ", "
                              << "max= " << customs.bpm.max;

  ofLogVerbose("  sett man ") << "\tOSC: " << customs.osc.status << ", "
                            << "host: " << customs.osc.host   << ", "
                            << "send: " << customs.osc.send   << ", "
                            << "listen: " << customs.osc.listen;

  ofLogVerbose("  sett man ") << "\tTUIO: status: " << customs.tuio.status << ", "
                            << "listen: " << customs.tuio.listen;

  ofLogVerbose("  sett man ") << "\taspect";
  ofLogVerbose("  sett man ") << "\t\tbackground color(" << customs.aspect.background.r << ", "
                              << customs.aspect.background.g << ", "
                              << customs.aspect.background.b << ")";

  ofLogVerbose("  sett man ") << "\t\tfbo color(" << customs.aspect.fbo.r << ", "
                              << customs.aspect.fbo.g << ", "
                              << customs.aspect.fbo.b << ")";

  ofLogVerbose("  sett man ") << "\t\tplayhead color(" << customs.aspect.playhead.r << ", "
                             << customs.aspect.playhead.g << ", "
                             << customs.aspect.playhead.b << ")";

  ofLogVerbose("  sett man ") << "\t\tdistorsion: moveX: " << customs.aspect.distorsion.moveX << ", "
                            << "moveY: " << customs.aspect.distorsion.moveY << ", "
                            << "rotX: "  << customs.aspect.distorsion.rotX  << ", "
                            << "rotZ: "  << customs.aspect.distorsion.rotZ  << ", "
                            << "scale: " << customs.aspect.distorsion.scale;

  ofLogVerbose("  sett man ") << "\tFIDUCIALS size: " << customs.fiducials.size << ", "
                            << "fadetimes (in: " << customs.fiducials.fadetime.in << ", "
                            << "out: " << customs.fiducials.fadetime.out << ")";

  //cout << "\t\tPercussions size: " << customs.fiducials.percussions.size << endl;
  ofLogVerbose("  sett man ") << "\t\tPercussions triggertime (in: " << customs.fiducials.percussions.triggertime.in << ", "
                            << "out: " << customs.fiducials.percussions.triggertime.out << ")";

  ofLogVerbose("  sett man ") << "\t\t\tkick  [fid " << customs.fiducials.percussions.kick.fid << "]\t"
                            << "color(" <<  customs.fiducials.percussions.kick.color.r << ", "
                            << customs.fiducials.percussions.kick.color.g << ", "
                            << customs.fiducials.percussions.kick.color.b << ")";

  ofLogVerbose("  sett man ") << "\t\t\tsnare [fid " << customs.fiducials.percussions.snare.fid << "]\t"
                            << "color(" <<  customs.fiducials.percussions.snare.color.r << ", "
                            << customs.fiducials.percussions.snare.color.g << ", "
                            << customs.fiducials.percussions.snare.color.b << ")";

  ofLogVerbose("  sett man ") << "\t\t\thihat [fid " << customs.fiducials.percussions.hihat.fid << "]\t"
                            << "color(" <<  customs.fiducials.percussions.hihat.color.r << ", "
                            << customs.fiducials.percussions.hihat.color.g << ", "
                            << customs.fiducials.percussions.hihat.color.b << ")";

  ofLogVerbose("  sett man ") << "\t\tmetro [fid " << customs.fiducials.metro.fid << "]\t"
                            << "color(" <<  customs.fiducials.metro.color.r << ", "
                            << customs.fiducials.metro.color.g << ", "
                            << customs.fiducials.metro.color.b << ")\n"
                            << "\t\t\tmeter: [angleRange " <<  customs.fiducials.metro.meter.angleRange << ", "
                            << "angleGap " <<  customs.fiducials.metro.meter.angleGap << ", "
                            << "radius " <<  customs.fiducials.metro.meter.radius << ", "
                            << "w " <<  customs.fiducials.metro.meter.w << "]\n"
                            << "\t\t\t\tbackColor (" <<  customs.fiducials.metro.meter.backColor.r << ", "
                            <<  customs.fiducials.metro.meter.backColor.g << ", "
                            <<  customs.fiducials.metro.meter.backColor.b << ")\n"
                            << "\t\t\t\tfrontColor (" <<  customs.fiducials.metro.meter.frontColor.r << ", "
                            <<  customs.fiducials.metro.meter.frontColor.g << ", "
                            <<  customs.fiducials.metro.meter.frontColor.b << ")";

  ofLogVerbose("  sett man ") << "\t\tbass [fid " << customs.fiducials.bass.fid << "]\t"
                            << "color(" <<  customs.fiducials.bass.color.r << ", "
                            << customs.fiducials.bass.color.g << ", "
                            << customs.fiducials.bass.color.b << ")";

  ofLogVerbose("  sett man ") << "\t\tpad [fid " << customs.fiducials.pad.fid << "]\t"
                            << "color(" <<  customs.fiducials.pad.color.r << ", "
                            << customs.fiducials.pad.color.g << ", "
                            << customs.fiducials.pad.color.b << ")";

  ofLogVerbose("  sett man ") << "\t\tchords:";
  ofLogVerbose("  sett man ") << "\t\t\tchord 1 [fid " << customs.fiducials.chords.chord1.fid << "]\t"
                            << "color(" <<  customs.fiducials.chords.chord1.color.r << ", "
                            << customs.fiducials.chords.chord1.color.g << ", "
                            << customs.fiducials.chords.chord1.color.b << ")";

  ofLogVerbose("  sett man ") << "\t\t\tchord 2 [fid " << customs.fiducials.chords.chord2.fid << "]\t"
                            << "color(" <<  customs.fiducials.chords.chord2.color.r << ", "
                            << customs.fiducials.chords.chord2.color.g << ", "
                            << customs.fiducials.chords.chord2.color.b << ")";


  ofLogVerbose("  sett man ") << "\t\t\tchord 3 [fid " << customs.fiducials.chords.chord3.fid << "]\t"
                            << "color(" <<  customs.fiducials.chords.chord3.color.r << ", "
                            << customs.fiducials.chords.chord3.color.g << ", "
                            << customs.fiducials.chords.chord3.color.b << ")";

  ofLogVerbose("  sett man ") << "\t\t\tchord 4 [fid " << customs.fiducials.chords.chord4.fid << "]\t"
                            << "color(" <<  customs.fiducials.chords.chord4.color.r << ", "
                            << customs.fiducials.chords.chord4.color.g << ", "
                            << customs.fiducials.chords.chord4.color.b << ")";

  ofLogVerbose("  sett man ") << "\t\t\tchord 5 [fid " << customs.fiducials.chords.chord5.fid << "]\t"
                            << "color(" <<  customs.fiducials.chords.chord5.color.r << ", "
                            << customs.fiducials.chords.chord5.color.g << ", "
                            << customs.fiducials.chords.chord5.color.b << ")";

  ofLogVerbose("  sett man ") << "\t\t\tchord 6 [fid " << customs.fiducials.chords.chord6.fid << "]\t"
                            << "color(" <<  customs.fiducials.chords.chord6.color.r << ", "
                            << customs.fiducials.chords.chord6.color.g << ", "
                            << customs.fiducials.chords.chord6.color.b << ")";

}



// struct SettingsManager::settings * SettingsManager::getDefaults() {
//   return &defaults;
// }

struct SettingsManager::settings * SettingsManager::getCustoms() {
  return &customs;
}
