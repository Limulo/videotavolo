#include "FiducialsManager.h"
//using namespace std;

FiducialsManager::FiducialsManager() {
	ofLogVerbose("  fid  man ") << "contructed!";

}

FiducialsManager::~FiducialsManager() {
	ofLogVerbose("  fid  man ") << "destroyed!";
}

void FiducialsManager::setup(int _w, int _h, struct SettingsManager::settings *_s, TimingThread *_thread, Sequencer *_seq, OscManager *_oscMan)
{
	ofLogVerbose("  fid  man ") << "setup!";

	// playground dimensions
  width = _w;
  height = _h;

	// get the reference to the settings
	s = _s;

	// obtain a reference to the timing thread
	// so to get information about the actual
	// bar index where we are every time
	thread = _thread;

	// obtain a reference to the sequencer in order
	// to add instrument to it and to print it
	// for debug purposes.
	seq = _seq;

  // obtain a reference to the OscManager
  // we will use later to get the level of audio
  // from PureData.
  oscMan = _oscMan;


	index = previous_index = -1;
	level = 0.0;
	bpm = 0;
	angleRange = 0.0;
	angleGap = 0.0;

	loadSettings();

	// Now it is time to initialize the 'udacn' tool.
	// Keep in mind that this must be done after
	// we have loaded our setting for the first time
	// in order to have the 'angle' and 'bpm' ranges.
	//
	// Now if we know the initial bpm value we must
	// pass it to the 'udacn' tool in order to have
	// a correct initialization of it but, prior
	// to do that, we must map it to the `angle` range
	// proprerly.
	int bpmRange = maxBpm - minBpm;
	float initialValue = (angleRange*(bpm -minBpm)) / bpmRange;
	udacn.setup( angleRange, initialValue);

	// TODO: vector reserve()
}

/**
 * The UPDATE method gets the index from the
 * surface manager
 */
void FiducialsManager::update( int _index )
{
	//ofLogVerbose("  fid  man ") << "update";

	index = _index;

	// thread->lock();
	// index = thread->index_8n;
  // //playheadPosition = thread->pos;
	// thread->unlock();

  // Here we display a sort of VU meter
  // in order to view level data coming from PD
  level = oscMan->getLevel();

	// update all the percissive fiducials
	// note: because we delete fiducials from the middle
	// of the vector when their fadeout animation has finished,
	// we increment the iterator only when necessary.
	vector< FiducialBase* >::iterator it;
	//for( it=fiducialVector.begin(); it!=fiducialVector.end(); ++it)
	for( it=fiducialVector.begin(); it!=fiducialVector.end(); )
	{
		// if the fiducial we are examining is dead,
		// remove it from the vector and get the next one
		if ( !(*it)-> isAlive() )
		{
			delete( *it );
			fiducialVector.erase( it );
			continue;
		}


		// 1) is the fiducial of "percission" type?
		// 2) if so, get the fiducial position inside the bar
		// 3) does the position correspond to the current index?
		// 4) if so, trigger the fiducial!

		// in order to see if the current fiducial is of percussive type
		// we can performa a dynamic cast and see if it is correctly executed
		//if( dynamic_cast<FiducialPercussion *> (*it) )
		// or, perform a simple check against the fiducial FID.
		if( (*it)->getFid() == FID_KICK ||
		 		(*it)->getFid() == FID_SNARE ||
				(*it)->getFid() == FID_HIHAT
			)
		{
			// if the current fiducial is of 'percussive' type
			// then call a fiducial-percussion-only method
			FiducialPercussion* fp = (FiducialPercussion*)(*it);

			// Do the following only if the fiducial is of type percussion:
			// if we are now playing a different rythmic value than before,
			// then all the percussive fiducial must be triggered
			if( index != previous_index && index == fp->getPosInBar() )
			{

				fp->trigger();
			}
		}
		else if( (*it)->getFid() == FID_BASS )
		{
			FiducialBass* fb = (FiducialBass*)(*it);
			fb->setLevel( level );
		}


		// call update on each fiducial inside the vector
		// doesn't matter the type.
		(*it)->update();

		// then increment the iterator.
		it++;
	}  // <-- endof for( it=fiducialVector.begin(); it!=fiducialVector.end(); )

	// after having looked at all the fiducials,
	// indexes must became equal in order
	// not to trigger them  more than
	// one time per musical interval.
	if( index != previous_index ) {
		previous_index = index;
	}
}

void FiducialsManager::draw()
{
	//ofLogVerbose("  fid  man ") << "draw";

	ofPushStyle();
	ofSetRectMode(OF_RECTMODE_CENTER);

	// draw all the fiducials
	vector< FiducialBase* >::iterator it;
	for( it=fiducialVector.begin(); it!=fiducialVector.end(); ++it)
	{
		(*it)->draw();
	}
	ofPopStyle();
}


void FiducialsManager::tuioAdd( long sid, int fid, float x, float y, float angle, float rspeed )
{
	ofLogVerbose("  fid  man ") << "tuio add (sid: "<< sid << ", fid: " << fid << ")";

	// create a new fiducial and keep track of it
	FiducialBase* fiducial;

	// a variable to keep track if the fiducial is
	// of percussion type (in that case we must to
	// particular operations).
	bool percussion = false;

	//same as before, this case we need to know
	// if the fiducial is of type 'metro'
	bool metro = false;

	// same as before: two new boleans to know when
	// we net a pad or some chords
	bool pad = false;
	bool chord=false;

	// this variable will be used to communicate
	// to the sequencer which plane is to be used
	// to store the instrument
	int instrument_plane = -1;

	if( fid == FID_KICK )
	{
		fiducial = new Kick( fid, sid, s );
		instrument_plane = 1;
		percussion = true;
	}
	else if ( fid == FID_SNARE )
	{
		fiducial = new Snare( fid, sid, s );
		instrument_plane = 2;
		percussion = true;
	}
	else if ( fid == FID_HIHAT )
	{
		fiducial = new Hihat( fid, sid, s );
		instrument_plane = 3;
		percussion = true;
	}
	else if ( fid == FID_METRO )
	{
		//ofLogWarning("  fid  man ") << "tuio add (fid: " << fid << ", sid: " << sid << "): adding a new one!";

		// The `metro` fiducial constructor is something different
		// from other fiducials constructors.
		// We pass it also a reference to the 'Udacn' tool and
		// the playground dimensions in order for it to
		// properly calculate the playground center.
		// TODO: note that while 'udacn' need the angleRange expressed in radians
		// the 'FiducialMetro' expects it to be expressed in degrees. Fix it!
		fiducial = new FiducialMetro( fid, sid, s, &udacn, width, height, (angleGap*360.0)/(2*PI) );
		metro = true;
	}
	else if ( fid == FID_BASS )
	{
		//ofLogWarning("  fid  man ") << "tuio add (fid: " << fid << ", sid: " << sid << "): adding a new one!";
		fiducial = new FiducialBass( fid, sid, s );
		oscMan->send("/bass", 1);
	}
	else if ( fid == FID_PAD )
	{
		//ofLogWarning("  fid  man ") << "tuio add (fid: " << fid << ", sid: " << sid << "): adding a new one!";
		fiducial = new FiducialPad( fid, sid, s );
		oscMan->send("/pad", 1);
		pad = true;
	}
	else if ( fid == FID_CHORD_1)
	{
		//ofLogWarning("  fid  man ") << "tuio add (fid: " << fid << ", sid: " << sid << "): adding a new one!";
		fiducial = new Chord1( fid, sid, s );
		oscMan->send("/chords/chord1", 1);
		chord = true;
	}
	else if ( fid == FID_CHORD_2 )
	{
		//ofLogWarning("  fid  man ") << "tuio add (fid: " << fid << ", sid: " << sid << "): adding a new one!";
		fiducial = new Chord2( fid, sid, s );
		oscMan->send("/chords/chord2", 1);
		chord = true;
	}
	else if ( fid == FID_CHORD_3 )
	{
		//ofLogWarning("  fid  man ") << "tuio add (fid: " << fid << ", sid: " << sid << "): adding a new one!";
		fiducial = new Chord3( fid, sid, s );
		oscMan->send("/chords/chord3", 1);
		chord = true;
	}
	else if ( fid == FID_CHORD_4 )
	{
		//ofLogWarning("  fid  man ") << "tuio add (fid: " << fid << ", sid: " << sid << "): adding a new one!";
		fiducial = new Chord4( fid, sid, s );
		oscMan->send("/chords/chord4", 1);
		chord = true;
	}
	else if ( fid == FID_CHORD_5 )
	{
		//ofLogWarning("  fid  man ") << "tuio add (fid: " << fid << ", sid: " << sid << "): adding a new one!";
		fiducial = new Chord5( fid, sid, s );
		oscMan->send("/chords/chord5", 1);
		chord = true;
	}
	else if ( fid == FID_CHORD_6 )
	{
		//ofLogWarning("  fid  man ") << "tuio add (fid: " << fid << ", sid: " << sid << "): adding a new one!";
		fiducial = new Chord6( fid, sid, s );
		oscMan->send("/chords/chord6", 1);
		chord = true;
	}
	else
	{
		ofLogWarning("  fid  man ") << "tuio add, fid " << fid << " not used!";
		// if the fiducial id is not one of that
		// are used by the application, return.
		return;
	}

	// do the following operations for every fiducials
	// doesn't matter the type of them
	fiducialVector.push_back( fiducial );
	fiducialVector.back()->tuioAdd( x*width, y*height, angle, rspeed );


	// do the following ops only if necessary:
	// that is only if the fiducial is of 'percussion' type
	if( percussion )
	{
		// conversion: 0-1 to 0-7
		int column = int(x * 8.0);
		((FiducialPercussion *)(fiducialVector.back()))->setPosInBar( column );

		// inform the sequencer about the new added fiducial
	  seq->tuioAdd(instrument_plane, column, sid);
	  //seq->print(0);
	}
	else if( metro )
	{
		// When a new 'metro' fiducial is added to the surface,
		// bind it to the 'udacn' tool, in order to make it control
		// the 'Timing thread' 'bpm' parameter.
		// To do this we pass it the 'angleRange' parameter and
		// the angle also.
		udacn.bind( angle );

		// When a new 'metro' is just added to the interactive surface
		// the BPM remains unchanged.
		// Changes on the BPM value happens when the 'metro' is rotated.
		// This means we don't need to update the animation freq of
		// any other fiducials yet (see the TuioUpdate method below).
	}
	else if( pad )
	{
		((FiducialPad*)(fiducialVector.back()))->getAnimator()->changeFreq( bpm / 60.0 );
	}
	else if( chord )
	{
		((FiducialChord*)(fiducialVector.back()))->getAnimator()->changeFreq( bpm / 60.0 );
	}
}


// we don't use this method yet
void FiducialsManager::tuioUpdate( long sid, int fid, float x, float y, float angle, float rspeed )
{
	ofLogVerbose("  fid  man ") << " tuio update (sid: "<< sid << ", fid: " << fid << ")";

	// see 'tuioAdd' for information on
	// these local variables
	bool percussion = false;
	bool metro = false;

	int instrument_plane = -1;
	if( fid == FID_KICK )
	{
		instrument_plane = 1;
		percussion = true;
	}
	else if ( fid == FID_SNARE )
	{
		instrument_plane = 2;
		percussion = true;
	}
	else if ( fid == FID_HIHAT )
	{
		instrument_plane = 3;
		percussion = true;
	}
	else if ( fid == FID_METRO )
	{
		//ofLogWarning("  fid  man ") << "tuio update, fid " << fid << " is of type 'METRO'!";
		metro = true;
	}
	else if ( fid == FID_BASS )
	{
		//ofLogWarning("  fid  man ") << "tuio update, fid " << fid << " is of type 'BASS'!";
	}
	else if ( fid == FID_PAD )
	{
		//ofLogWarning("  fid  man ") << "tuio update, fid " << fid << " is of type 'PAD'!";
	}
	else if ( fid == FID_CHORD_1 ||
						fid == FID_CHORD_2 ||
						fid == FID_CHORD_3 ||
						fid == FID_CHORD_4 ||
						fid == FID_CHORD_5 ||
						fid == FID_CHORD_6 )
	{
		//ofLogWarning("  fid  man ") << "tuio update, fid " << fid << " is of type 'CHORD'!";
	}
	else
	{
		ofLogWarning("  fid  man ") << "tuio update, fid " << fid << " not used!";
		// if the fiducial id is not one of that
		// are used by the application, return.
		return;
	}


	bool found = false;
	// update the corresponding fiducial in the list
	vector< FiducialBase* >::iterator it;
	for( it=fiducialVector.begin(); it!=fiducialVector.end(); ++it)
	{
		if( (*it)->getSid() == sid )
		{
			// se have found the session id we are looking for
			found = true;

			// do the following ops for every fiducial
			// doesn't matter the type
			(*it)->tuioUpdate( x*width, y*height, angle, rspeed );

			// do something particular only if the fiducial
			// is of 'percussive' type
			if( percussion )
			{
				// conversion: 0-1 to 0-7
				int column = int(x * 8.0);

				//(*it)->tuioUpdate( x*width, y*height, column );
				((FiducialPercussion *)(*it))->setPosInBar( column );

				// inform the sequencer about the update of the fiducial
				seq->tuioUpdate(instrument_plane, column, sid );
				//seq->print(0);
			}
			else if( metro )
			{
				// Ok, now the 'metro' fiducial on the surface is moving so we
				// must update the 'Timing thread' 'bpm' accordingly.
				// In order to do that we must
				// 1) first call the 'udacn' update method on the 'udacn' tool;
				// 2) then scale its normalized output to the actual 'bpm' range;
				// 3) write the value on the 'Timing thread';
				udacn.update( angle, rspeed );
				bpm = int( (maxBpm-minBpm)*udacn.getOutput() ) + minBpm;

				thread->lock();
				thread->bpm = bpm;
				thread->unlock();

				// TODO: when the 'metro' is rotated we need also to update
				// all the animation which must remain in sync whit the
				// overall rythm. See the last line of this function.
			}
			// if we found the SID so there isn't anything
			// to look for anymore.
			break;
		} /* end of if( (*it)->getSid() == sid ) */
	}   /* end of for( it=fiducialVector.begin(); it!=fiducialVector.end(); ++it) */

	if( !found )
	{
		ofLogWarning("  fid  man ") << "tuio update, sid " << sid << " not found!";
		return;
	}

	// If we have just found a `metro` fid being updated,
	// we must also update all the other fiducials which
	// depends on the bpm to synchronize their own animations.
	if( metro )
	{
		for( it=fiducialVector.begin(); it!=fiducialVector.end(); ++it)
		{
			int type = (*it)->getFid();
			// Check if there are some fiducial which depends
			// on the overall frequency for their own animations.
			if( type == FID_PAD )
			{
				// found a fiducial which freq must be updated
				// TODO: update the frequency parameter of the fiducials
				((FiducialPad*)(*it))->getAnimator()->changeFreq( bpm / 60.0 );
			}
			else if( type == FID_CHORD_1 ||
							 type == FID_CHORD_2 ||
							 type == FID_CHORD_3 ||
							 type == FID_CHORD_4 ||
							 type == FID_CHORD_5 ||
							 type == FID_CHORD_6
						 )
			{
				((FiducialChord*)(*it))->getAnimator()->changeFreq( bpm / 60.0 );
			} /* end of if( type == ...) */
		}   /* end of for */
	}     /* end of if( metro ) */
}


void FiducialsManager::tuioRemove( long sid, int fid )
{
	ofLogVerbose("  fid  man ") << "tuio remove (sid: " << sid << ")";

	bool percussion = false;

	if( fid == FID_KICK || fid == FID_SNARE || fid == FID_HIHAT )
	{
		//ofLogWarning("  fid  man ") << "tuio remove, fid " << fid << " is of type 'PERCUSSION'!";
		percussion = true;
	}
	else if ( fid == FID_METRO )
	{
		//ofLogWarning("  fid  man ") << "tuio remove, fid " << fid << " is of type 'METRO'!";
	}
	else if ( fid == FID_BASS )
	{
		//ofLogWarning("  fid  man ") << "tuio remove, fid " << fid << " is of type 'BASS'!";
		oscMan->send("/bass", 0);
	}
	else if ( fid == FID_PAD )
	{
		//ofLogWarning("  fid  man ") << "tuio remove, fid " << fid << " is of type 'PAD'!";
		oscMan->send("/pad", 0);
	}
	else if ( fid == FID_CHORD_1 )
	{
		//ofLogWarning("  fid  man ") << "tuio remove, fid " << fid << " is of type 'CHORD 1'!";
		oscMan->send("/chords/chord1", 0);
	}
	else if ( fid == FID_CHORD_2 )
	{
		//ofLogWarning("  fid  man ") << "tuio remove, fid " << fid << " is of type 'CHORD 2'!";
		oscMan->send("/chords/chord2", 0);
	}
	else if ( fid == FID_CHORD_3 )
	{
		//ofLogWarning("  fid  man ") << "tuio remove, fid " << fid << " is of type 'CHORD 3'!";
		oscMan->send("/chords/chord3", 0);
	}
	else if ( fid == FID_CHORD_4 )
	{
		//ofLogWarning("  fid  man ") << "tuio remove, fid " << fid << " is of type 'CHORD 4'!";
		oscMan->send("/chords/chord4", 0);
	}
	else if ( fid == FID_CHORD_5 )
	{
		//ofLogWarning("  fid  man ") << "tuio remove, fid " << fid << " is of type 'CHORD 5'!";
		oscMan->send("/chords/chord5", 0);
	}
	else if ( fid == FID_CHORD_6 )
	{
		//ofLogWarning("  fid  man ") << "tuio remove, fid " << fid << " is of type 'CHORD 6'!";
		oscMan->send("/chords/chord6", 0);
	}
	else
	{
		ofLogWarning("  fid  man ") << "tuio remove, fid " << fid << " not used!";
		// if the fiducial id is not one of that
		// are used by the application, return.
		return;
	}

	// delete the corresponding fiducial from the list
	vector< FiducialBase* >::iterator it;
	for( it=fiducialVector.begin(); it!=fiducialVector.end(); ++it)
	{
		if( (*it)->getSid() == sid )
		{
			(*it)->tuioRemove();

			if( percussion )
			{
				// inform the sequencer about the deletion of the fiducial
				seq->tuioRemove( sid );
			}
			// we need also to remove the fiducial from the vector
			// TODO: we must pay attention to this because we want to
			// play the "disappear" animation before the object being
			// actually removed from the vector
			//delete( *it );
			//fiducialVector.erase( it );
			return;
		}
	}
	ofLogWarning("  fid  man ") << "tuio removed, sid " << sid << " not found!";
}



int FiducialsManager::getBpm()
{
	return bpm;
}

int FiducialsManager::getFiducialsNumber() {
	return fiducialVector.size();
}





void FiducialsManager::saveSettings()
{
	ofLogVerbose("  fid  man ") << "save settings";
	// do nothing for now
}

void FiducialsManager::loadSettings()
{
	ofLogVerbose("  fid  man ") << "load settings";
	// Every new fiducial created on the surface
	// is responsible to get its own settings from itself, so
	// fiducial manager won't call its `loadSettings` method.

	// Instead, fiducial Manager must load some settings
	// regarding fiducials IDs:
	FID_KICK 	 = s->fiducials.percussions.kick.fid;
	FID_SNARE	 = s->fiducials.percussions.snare.fid;
	FID_HIHAT  = s->fiducials.percussions.hihat.fid;
	FID_BASS	 = s->fiducials.bass.fid;
	FID_PAD	   = s->fiducials.pad.fid;
	FID_METRO	 = s->fiducials.metro.fid;
	FID_CHORD_1= s->fiducials.chords.chord1.fid;
	FID_CHORD_2= s->fiducials.chords.chord2.fid;
	FID_CHORD_3= s->fiducials.chords.chord3.fid;
	FID_CHORD_4= s->fiducials.chords.chord4.fid;
	FID_CHORD_5= s->fiducials.chords.chord5.fid;
	FID_CHORD_6= s->fiducials.chords.chord6.fid;

	// do the same the 'Timing thread' does:
	// load the defautl settings for the bpm
	bpm = s->bpm.value;
	// Then load the min and max values for the 'bpm'
	// in order to correctly map the 'metro' fiducials
	// rotation.
	minBpm = s->bpm.min;
	maxBpm = s->bpm.max;

	// Now get the excusion from the 'metro' ficucial settings.
	angleRange = s->fiducials.metro.meter.angleRange * PI;
	angleGap   = s->fiducials.metro.meter.angleGap   * PI;
}


/*
void FiducialsManager::loadSettings() {
	// First load settings for the Fiducial Manager itself

	// nothigh to load for the moment

	// Then call the appropriate method to
	// load setting for all the fiducials.

	vector< FiducialBase* >::iterator it;
	//for( it=fiducialVector.begin(); it!=fiducialVector.end(); ++it)
	for( it=fiducialVector.begin(); it!=fiducialVector.end(); ++it )
	{
		// 1) find the type of the fiducial we are examining
		// 2) then cast the pointer to the specific type
		// 3) call the appropriate method for the type
		if( dynamic_cast<Kick *> (*it) )
		{
			// if dynamic_cast succeed! call some
			// fiducial-percussion-only methods
			Kick* fp = (Kick*)(*it);
			fp->loadSettings();
		}
		else if( dynamic_cast<Snare *> (*it) )
		{
			// if dynamic_cast succeed! call some
			// fiducial-percussion-only methods
			Snare* fp = (Snare*)(*it);
			fp->loadSettings();
		}
		else if( dynamic_cast<Hihat *> (*it) )
		{
			// if dynamic_cast succeed! call some
			// fiducial-percussion-only methods
			Hihat* fp = (Hihat*)(*it);
			fp->loadSettings();
		}
	}
}
*/



// this are alterante methods to add and remove fiducials
// in order to have another way to debug the application
// bool FiducialsManager::inside( float _x, float _y )
// {
// 	float x = _x * width;
// 	float y = _y * height;
//
// 	vector< FiducialBase* >::iterator it;
// 	for( it=fiducialVector.begin(); it!=fiducialVector.end(); ++it)
// 	{
// 		if( (*it)->inside(x, y) )
// 			return true;
// 	}
// 	return false;
// }
//
//
//
// int FiducialsManager::getSidFromPos( float _x, float _y )
// {
// 	float x = _x * width;
// 	float y = _y * height;
//
// 	vector< FiducialBase* >::iterator it;
// 	for( it=fiducialVector.begin(); it!=fiducialVector.end(); ++it)
// 	{
// 		if( (*it)->inside(x, y) )
// 			return (*it)->getSid();
// 	}
// 	return -1;
// }
