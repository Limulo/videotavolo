Here we are collectiong all the observations we make during the development.

## NOTES

* OscReceiver uses a separate _thread_ by itself.
* Every object is responsible to retrieve its own settings by itself from the reference to the _settings struct_ it has. The main app must only guaratee that these settings are updated, loading them from the XML file at startup and every time the user wants to load them.
* Every time you load new settings from the XML, keep in mind that fiducials settings will be applied to newly created fiducials and they wouldn't apply to already created fiducials!
* we use multiple `ofLog` _modules_ to output different logging information coming from different sections of the program;
* The `Udacn` tool belongs to the `FiducialsManager` which is responsible to update it and get its output and transfer it, scaled or not, respectively to the `Timing Thread` and to the current `Metro Fiducial`.
The `Timing Thread` will use it to update the reproduction speed/rythm, etc... while, on the other hand, the `Metro Fiducial` will use the value to correctly draw a graphical representation.

## TODO

a list of thing to be done:
* modify the way sequencer gets its reference to the OscManager (pass by reference instead). (similar for SurfaceManager, FiducialsManager)
* _TimingThread_ is usign `sleep()`. Is that the best way to achieve our target?
* is a `vector< FiducialBase* >` (FiducialBase pointers vector) the best way to do this? Can a _list_ do the job? 
* is it possible to access vector elements with a simple _array synthax_: `for(int i=0; i<vectro,size(); i++)`.
* give a name to the App Window (use `ofSetWindowTitle("gianni")`);
* give a name to threads
* add PLAY, STOP, buttons to the Timeline
* instance parametere initialization.
* try to implement your own TUIO wrapper instead of using `TUIO Manager` AND `ofxTUIO`;
* correct the app BPM and actual BPM divergence!
* there's a problem in rendering the playhead graphics, it seems the playheadline move in _jerks_, going more to the right side than the place where it shoud be. Maybe its a problem related to the fact here we have some data exchange between different threads.
* find a better way to use constructor/deconstructor: you see, some of them (that of 'FiducialManager' or of 'Udacn') are not called because of a particular way of creating the object (we are not explicitly calling the 'new' operator).
* Create a single class which will menage all the different animations. A sort of unique _procedural animation oscillator_.
* Add a _sync_ function to all the animators in order to make the animation fully synchronized to the main clock time instead of starting their animation indipendently one from the other.
* adda visual feedback every time we hit some "save/load" key.

## DONE

* FiducialBase: `setup` and `add` joined in a single method
* XML to load one time settings (HOST, PORT, colors, etc...)
* XML to save/load in-place setting (mesh rotations, translations, scales, etc...)
* add a way to enable TUIO send/receive in runtime;
* improve the _module_ names
* change the way _settings_ are printed out to the console in the verbose output of the app.

---

## 2017-10-11
* some modification to the code: trying to create somethinf to have a meter for the Metro fiducial;
* test display moved inside surface manager

## 2017-10-10

* **Animator ASR** class moved inside the Fiducial Base class because every fiducial, despite its type, need a _fade in_ and _fade out_ animation!
* added **Bass**, **Pad**, **Chords** and **Metro**;
* moved some debug routine to the base class;
* implemented rotation for all the fiducials;
There are 2 different way to see if a fiducial is of type percussion
* if( dynamic_cast<FiducialPercussion *> (*it) )
* if( (*it)->getFid() == FID_KICK || (*it)->getFid() == FID_SNARE || (*it)->getFid() == FID_HIHAT  )

## 2017-10-06

Conversion problem between _pixels_ and _cm_ (see the [website article](http://www.limulo.net/website/coding/graphics/pixels-vs-real-dimensions.html))!

## 2017-09-29

* found the bug in the fullscreen in the main monitor. This is the console output:
```
Failed to open BO for returned DRI2 buffer (1440x900, dri2 back buffer, named 14).
This is likely a bug in the X Server that will lead to a crash soon.
Segmentation fault
```
* integration of Fbo to the main application
* found a subtle bug: there's an imprecision in calculating the time so when setting the BPM
(both to 120 or 60 BPM), app BPM and actual BPM will progressively diverge!


## 2017-09-28

Work on the fbo transformation to correct the anamorphic distorsion.

## 2017-09-27

## 2017-09-25

* Error: Floating point exception

## 2017-09-23

* now OSC Manager can deal with interactively loading settings from the XML file;
* now OSC Manager can be turne ON/OFF interactively;

## 2017-09-22

Problems to be solved:
1. TuioManager/MouseManager: use a list instead of a vector in order to be faster in retrieve sids and remove them
2. graphical interface: where to store setting and let them be available for everyone
3. study a better way to add remove fiducials usign the mouse
5. test XML setting loading/saving.

### FIXED

4. fix the bug of ficucial with strange dimensions: it seems to be caused by the fact the fiducial is created near the playhead while it is passing by.
It was caused by a lacking initialization inside the _Animator_ classes.

## 2017-09-16

* an `Aborted` message is created when the method `ofXml.addValue` wants to add a value inside the XML file which apparently hasn't been correctly created yet. The console message says:
```
terminate called after throwing an instance of 'Poco::NullPointerException'
  what():  Null pointer
Aborted
```
This information can be useful to know what does `Aborted` mean (we can look for this term inside the _ofXml_ source code).

## 2017-09-15

* The application has been left free to run and it isn't crashed but OSC messages seems not to arrive to PD anymore. Same thing seem to have happened for OSC messages outcoming from PD. This is happened after the monitor have been gone to stop for two times.

## 2017-09-14

### timeThread_and_OSC_sequencer-2

### timeThread_and_OSC_sequencer-1

1. The application crashes after a few minutes. Messages that are collected are `Aborted` and then `Segmentation Fault`;
2. The problem appears to be solved (the appa ha worked continuously for 1h15m prior to be closed manually). The solution has been to move the `ofxMessage` inside the local scope of the function which need it. Before the message was an instance variable and it was used by anyone, also by some threads who needed it (like the method `send`, invoked by _Sequencer_, in turn invoked by the _timing thread_.
