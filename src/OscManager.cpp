#include "OscManager.h"
//using namespace std;

OscManager::OscManager() {
	ofLogVerbose("  osc  man ") << "contructed!";
}

OscManager::~OscManager() {
	ofLogVerbose("  osc  man ") << "destroyed!";
}

void OscManager::setup( SettingsManager::settings *_s )
{
  ofLogVerbose("  osc  man ") << "setup";

	s = _s;

	loadSettings(); // one time loading
}

//void OscManager::send(string s, int & i)
void OscManager::send(string s, int i)
{
	// send OSC messages only
	//if the status is "on".
	if( STATUS )
	{
		ofxOscMessage m;
		//ofLogVerbose() << "OSC MANAGER SEND: " << s << ", " <<  i << ";";
		m.clear();
		m.setAddress( s );
		m.addIntArg( i );

		//m.addFloatArg(3.5f);
		//m.addStringArg("hello");
		//m.addFloatArg(ofGetElapsedTimef());
		sender.sendMessage( m, false );
	}
}


// this method was previously known as "receive"
void OscManager::update()
{
	ofxOscMessage m;
  while( receiver.hasWaitingMessages() )
  {
    // get the next message
    m.clear();
    receiver.getNextMessage( &m );

		// process the incoming OSC messages
		// only if status is "on"
		if( STATUS ) {

			if( m.getAddress() == "/bass/level")
			{
				level = m.getArgAsFloat(0);
				//cout << "level: " << level << endl;
			}
		}
  }
}

float OscManager::getLevel()
{
	return level;
}

// not used fro the moment
void OscManager::saveSettings()
{
	ofLogVerbose("  osc  man ") << "save settings";
	// do nothing
}

void OscManager::loadSettings()
{
	ofLogVerbose("  osc  man ") << "load settings";

	if( s->osc.status == "on" )
	{
		STATUS = true;
		ofLogNotice("  osc  man ") << "status ON!";
	}
	else
	{
		STATUS = false;
		ofLogNotice("  osc  man ") << "status OFF";
	}

	HOST 					= s->osc.host;
	SEND_PORT 		= ofToInt( s->osc.send );
	LISTEN_PORT 	= ofToInt( s->osc.listen );

	// it would be better to reset all elements
	// which depends on incoming OSC messages
	level = 0.0;

	// Call these two methods every time
	// there's a change in the address or the port
	// used to receive/send messages.
	//
	// Looking at the source code for 'OscSender' and 'OscReceiver'
	// the 'setup' method seems to be the correct methods
	// to use in case we want to change parameters
	// like these.
	sender.setup( HOST, SEND_PORT);
	// enable the OSC messages reception
	receiver.setup( LISTEN_PORT );
}


void OscManager::swap()
{
	STATUS = !STATUS;
	if( STATUS )
		ofLogNotice("  osc  man ") << "swap: status ON!";
	else
		ofLogNotice("  osc  man ") << "swap: status OFF";
}
