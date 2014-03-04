#include "AppCore.h"

#define KICK_VOL		0.65
#define SNARE_VOL		0.04
#define HIHAT_VOL		0.20
#define DRUM_VOL		0.8
#define SNARE_DLY_VOL	0.5
#define PADS_VOL		0.30
#define BASS_VOL		0.95

using namespace std;

// === APPCORE SETUP ======================================================================
void AppCore::setup(const int numOutChannels, const int numInChannels, const int sampleRate, const int ticksPerBuffer) 
{

	// double check where we are ...
	ofLog() << "\tAPPCORE SETUP: " << ofFilePath::getCurrentWorkingDirectory();
	
	
	if(!pd.init(numOutChannels, numInChannels, sampleRate, ticksPerBuffer)) 
	{
		ofLog(OF_LOG_ERROR) << "\tAPPCORE SETUP: Could not init pd";
		OF_EXIT_APP(1);
	}
	
	// receive source names
	pd.subscribe("loadbang");
	pd.subscribe("kick");
	pd.subscribe("snare");
	pd.subscribe("hihat");
	pd.subscribe("kick_vol");
	pd.subscribe("snare_vol");
	pd.subscribe("hihat_vol");
	pd.subscribe("croma_time");
	//pd.addSource("start_seq");
	pd.subscribe("bass");
	pd.subscribe("pads");
	pd.subscribe("bass_vol");
	pd.subscribe("pads_vol");
	//pd.addSource("sync_1");
	pd.subscribe("chords");
	pd.subscribe("colon_bang");
	pd.subscribe("snare_dly_vol");
	pd.subscribe("drum_vol");
	pd.subscribe("bass_lvl_to_OF"); // originariamente pensato per fare pulsare il colore rosso a tempo di musica
	
	
	// add listener (per il send dallo sketch di PureData)
	pd.addReceiver(*this);
	pd.ignore(*this);
	pd.receive(*this, "bass_lvl_to_OF");	// Ascolta da PD
		
	// add the data/pd folder to the search path
	//pd.addToSearchPath("little_synth");
	
	// audio processing on
	pd.start();
	
	// open patch
	Patch patch = pd.openPatch("little_synth/synth.pd");
	ofLog() << "\tAPPCORE SETUP: " << patch;
	
	pd.sendBang("loadbang");
	pd.sendFloat("kick_vol", KICK_VOL);
	pd.sendFloat("snare_vol", SNARE_VOL );
	pd.sendFloat("hihat_vol", HIHAT_VOL);
	pd.sendFloat("pads_vol", PADS_VOL);
	pd.sendFloat("bass_vol", BASS_VOL );
	pd.sendFloat("snare_dly_vol", SNARE_DLY_VOL);
	pd.sendFloat("drum_vol", DRUM_VOL);
	
	ofLog() << "\tAPPCORE SETUP: finish" << endl;

}

// === APPCORE UPDATE =====================================================================
void AppCore::update() {}


// === APPCORE DRAW =======================================================================
void AppCore::draw() {}


// === APPCORE EXIT =======================================================================
void AppCore::exit() 
{
	pd.closePatch("little_synth/synth.pd");
	cout << "APPCORE EXIT: finish - Ending PD" << endl;
}


// === APPCORE OTHER ======================================================================
void AppCore::audioReceived(float * input, int bufferSize, int nChannels) {
	pd.audioIn(input, bufferSize, nChannels);
}

void AppCore::audioRequested(float * output, int bufferSize, int nChannels) {
	pd.audioOut(output, bufferSize, nChannels);
}

void AppCore::print(const std::string& message) {
	cout << message << endl;
}


void AppCore::receiveFloat(const std::string& dest, float value) {
	if (dest=="bass_lvl_to_OF") {
		bass_lvl = value; // * 0.0175; 
		//bass_lvl = ofClamp(value, 0.0, 1.0);
		//cout << bass_lvl << "\n" << endl;
	}
}


void AppCore::bang(const std::string& receive) {
	pd.sendBang(receive);
}


void AppCore::send_float(const std::string& receive, float val) {
	pd.sendFloat(receive, val);
}


