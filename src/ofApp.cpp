#include "ofApp.h"
using namespace std;

// version
int vMajor = 1;
int vMinor = 6;
int vMaintenance = 0;

// colori generici
ofColor black	(0,		0,		0);
ofColor grey	(200,	200,	200);
ofColor rosso	(255,	0,		0);
ofColor green	(0,		255,	0);
ofColor blue	(0,		0,		255);
ofColor white	(255,	255,	255);

// colori estrapolati da una palette creata ad-hoc
ofColor c1		(123,	189,	224);
ofColor c2		(117,	196,	141);
ofColor c3		(132,	60,		42);
ofColor c4		(74,	155,	85);
ofColor c5		(163,	155,	85);
ofColor c6		(50,	105,	209);
ofColor c7		(59,	144,	165);
ofColor c8		(204,	150,	2);
ofColor c9		(37,	96,		54);

// assegno, a ciascun componente grafico i colori della palette
ofColor color_kick		= c3;
ofColor color_snare		= c6;
ofColor color_hihat		= c7;
ofColor color_sfondo	= c1;
ofColor color_playhead	= c8;
ofColor color_bpm		= c5;
ofColor color_bpm_corona = c4;
ofColor color_bass		= c9;
ofColor color_pads		= c2;

// ulteriore palette colori generata per differenzioare tra loro i vari accordi
ofColor color_accordo_0 = ofColor(77,	15,		52);	// Am
ofColor color_accordo_1 = ofColor(15,	76,		39);	// G
ofColor color_accordo_2 = ofColor(76,	70,		15);	// C
ofColor color_accordo_3 = ofColor(15,	21,		76);	// Em
ofColor color_accordo_4 = ofColor(34,	76,		15);	// F
ofColor color_accordo_5 = ofColor(76,	15,		40);	// D

//da Palette 2 - mode 0
ofColor verde	(68,	181,	16);
ofColor giallo	(221,	201,	48);
ofColor	azzurro (247,	54,		89);


// SETUP ///////////////////////////////////////////////////////////////
void ofApp::setup() 
{
	
	//ofLogToFile("myLogFile.txt", false);
	//ofLog(OF_LOG_VERBOSE);
	//ofLog() << "VIDEOTAVOLO LOG - MAIN SETUP: start" << endl;
	cout << "VIDEOTAVOLO LOG - MAIN SETUP: start" << endl;
	
	//------------------ SISTEMA --------------- //
	ofSetVerticalSync(true);
	ofSetFrameRate(FPS);
	
	//------------------ PLAY ------------------ //
	matrice.init();
	
	//for (int i=0; i<PLANES; ++i)
	//{
	//	matrice.print_table(i);
	//	cout << "\n";
	//}
	
	//Fid_Base::fps = FPS;
	
	// inizializzazione variabili di temporizzazione e gestione della messa in play
	initial_time = ofGetSystemTime();
	bpm = 120;					// BPM di default
	Fid_Synth::synth_bpm = bpm; // BPM di default
	croma_time=(60000 / (bpm*2)); // tempo di una croma in millisecondi 
	// 1 minuto = 60000 ms; 
	// 60000 ms / bpm = tempo di 1 semiminima; 
	// 60000 ms / (bpm*2) = tempo di 1 croma
	
	n_crome = 0;						// contatore di quante crome sono state messe in play fino ad ora
	last_croma_time = 8 * croma_time;	// una moltiplicazione per otto per far cominciare i conteggi dopo la prima battuta
	
	// teniamo traccia del tempo trascorso dall'apertura del programma
	hours = minutes = seconds = 0;
	
	
	// ----------------- SONORO ---------------- //
	//ofLog() << "LibPD not defined ---> OSC utilities SETUP" << endl;
	cout << "LibPD not defined ---> OSC utilities SETUP" << endl;
	// open an outgoing connection to HOST:PORT
	sender.setup(HOST, S_PORT);
	receiver.setup(R_PORT);
	ofxOscMessage m;
	m.setAddress("/transport/croma_time");
	m.addFloatArg((float)croma_time);
	sender.sendMessage(m);
	m.clear();
	m.setAddress("/transport/fps");
	m.addIntArg(FPS);
	sender.sendMessage(m);
	m.clear();
	livello_audio_basso = 0.0;	
	
	//------------------TUIO------------------ //
	ofAddListener(tuio.objectAdded,   this, &ofApp::objectAdded);
	ofAddListener(tuio.objectRemoved, this, &ofApp::objectRemoved);
	ofAddListener(tuio.objectUpdated, this, &ofApp::objectUpdated);
	ofAddListener(tuio.cursorAdded,	  this, &ofApp::cursorAdded);
	ofAddListener(tuio.cursorRemoved, this, &ofApp::cursorRemoved);
	ofAddListener(tuio.cursorUpdated, this, &ofApp::cursorUpdated);
	
	tuio.start(TUIO_PORT); 
	
	
	//------------------ UTILITA' -------------- //
	// per la resa a schermo distorta
	margine = 100;
	quadro.setWidth(768);
	quadro.setHeight(768);
	wQuadro = quadro.getWidth();
	hQuadro = quadro.getHeight();
	
	wFbo  = hFbo  = wQuadro + 2*margine; 
	wMesh = hMesh = wFbo; 
	wWindow = ofGetWindowWidth();
	hWindow = ofGetWindowHeight();
	
	meshReset(); // inizializzo i valori della mesh a quelli hard coded
	//rvReset(); // inizializzo le trasformazioni sulle coordinate TUIO

	// mesh
	mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	// mesh vertexes
	int latomezzi = wMesh*0.5;
	ofVec3f nw(-latomezzi, -latomezzi, 0);
	ofVec3f ne(+latomezzi, -latomezzi, 0);
	ofVec3f se(+latomezzi, +latomezzi, 0);
	ofVec3f sw(-latomezzi, +latomezzi, 0);
	// texture coords
	ofVec2f nwi(0,0);
	ofVec2f nei(wFbo, 0);
	ofVec2f sei(wFbo, hFbo);
	ofVec2f swi(0, hFbo);
	// mesh construction
	mesh.addVertex(nw);
	mesh.addTexCoord(nwi);
	mesh.addVertex(ne);
	mesh.addTexCoord(nei);
	mesh.addVertex(sw);
	mesh.addTexCoord(swi);
	mesh.addVertex(se);
	mesh.addTexCoord(sei);
	// allochiamo la memoria necessaria per FBO e TEXTURE
	fbo.allocate(wFbo, hFbo, GL_RGB);
	text.allocate(wFbo, hFbo, GL_RGB);
	
	
	
	//------------------ PLAY HEAD ------------- //
	playHeadPosition = 0;
	
	int larghezza = wQuadro/NCOLUMNS;
	for(int i = 0; i < NCOLUMNS; ++i) {
		striscie[i].init(0+i*larghezza, 0-margine, larghezza, hFbo, color_playhead);
	}
	
	

	//------------------ IMMAGINE LOGHI -------- //
	fboLogosFilling(wQuadro, hQuadro);
	
	
	//------------------ VARIE ----------------- //
	centro.x = wQuadro/2.0f;
	centro.y = hQuadro/2.0f;
	
	bSetup = true;	// normalmente, appena acceso il programma siamo in modalità setup
	bDebug = false;	// di default debug è disattivato
	
	angolo = 0.0f;
	rot_vel = 0.0f;
	memoria_bpm_angle = 0.0f;
	
	digit.setup();
	
	//ofLog(OF_LOG_SILENT);
	
}

// UPDATE //////////////////////////////////////////////////////////////
void ofApp::update() 
{
	//printf("%d %d %d\n", hours, minutes, seconds);
	// teniamo traccia di quanto tempo trascorso dall'apertura del programma
	// tempo dall'apertura del programma espresso in secondi
	seconds = ofGetElapsedTimeMillis() / 1000; 
	minutes = seconds / 60 ;
	hours	= minutes / 60 ;
	minutes = minutes % 60;
	seconds = seconds % 60;	
	
	
	if(bSetup) 
	{
		// se siamo in fase di setup iniziale 
		
		//core.update();
		tuio.getMessage();
				
		// chiamo il metodo UPDATE CONTINUOUS per i fiducial rotativi, quadrati e rotondi
		for (vector<Fid_Rot*>::iterator it=rot_vec.begin(); it !=rot_vec.end(); ++it) 
			(*it)->update_continuos(0);
		
		for (vector<Fid_Sqr*>::iterator it=sqr_vec.begin(); it !=sqr_vec.end(); ++it) 
			(*it)->update_continuos(0);
	
		for (vector<Fid_Round*>::iterator it=rnd_vec.begin(); it !=rnd_vec.end(); ++it) 
			(*it)->update_continuos(0);
		
	} 
	else 
	{
		// se invece stiamo utilizzando il programma in modo normale

		tuio.getMessage();
		actual_time = ofGetSystemTime(); 
		time = actual_time - initial_time; //tempo trascorso dall'apertura del programma
		croma_time = (60000 / (bpm*2));
		
		// TIME CONTROL ------------------------------------------------
		if(time - last_croma_time >= croma_time) //(n_crome!=storico_crome)
		{
			n_crome = n_crome%8;
			play(n_crome);
			
			// aggiorno i valori per la visualizzazione corretta dell'animazione del Fid Synth
			// questo avviene in corrispondenza delle sole crome
			Fid_Synth::synth_bpm = bpm;
			for (vector<Fid_Synth*>::iterator it=synth_vec.begin(); it !=synth_vec.end(); ++it) 
				(*it)->reset_internal_timer(n_crome);
			
			last_croma_time=time;
			n_crome++;
		}
		
		ofxOscMessage m;
		m.setAddress("/transport/croma_time");
		m.addFloatArg((float)croma_time);
		sender.sendMessage(m);

		// CENTER: update della posizione del contro tavolo -----------
		centro.x = wQuadro/2.0f;
		centro.y = hQuadro/2.0f;
	
		// chiamo il metodo UPDATE CONTINUOUS per tutti i fiducial ----
		for (vector<Fid_Bass*>::iterator it=bass_vec.begin(); it !=bass_vec.end(); )
		{ 
			
			
			//cout << "INIZIO il controllo dei messaggi OSC:" << endl;
			while( receiver.hasWaitingMessages() ){		    
				// get the next message
				ofxOscMessage m;
				receiver.getNextMessage(&m);
		
				// check for bass level message
				if(m.getAddress() == "/bass/level") {
					// il messaggio OSC che ricevo da PureData potrebbe essere un valore FLOAT o INT
					// faccio un controllo sul tipo di messaggio per operare la corretta conversione
					// (in realtà non sarebbe necessario visto che  i metodi 'getArgAs...' della classe
					// 'ofxOscMessage' già operano lo stesso tipo di controllo ).
					//cout << "\tecco un messaggio in attesa d'essere processato - arg di tipo "<< m.getArgTypeName( 0 ) << " ( "<< m.getArgType( 0 ) << " ) - " << endl;
					if( m.getArgType( 0 ) == 1 ) {
						livello_audio_basso = (float)m.getArgAsInt32(0);
					} else {
						livello_audio_basso = m.getArgAsFloat(0);
					}
					//cout << "\timposto la variabile 'livello_audio_basso' al valore: " << livello_audio_basso << endl;
				}
					
			}
			//if(livello_audio_basso == 0)
			//	cout << "\n" << endl;
			
			if( !(*it)->isAlive() ) {
				delete (*it);
				bass_vec.erase(it);	
			} else {
				//cout << "UPDATE DEL FIDUCIAL con il valore: " << livello_audio_basso << endl;
				(*it)->update_continuos(livello_audio_basso);
				it++;
			}
			
		}
		
		//for (vector<Fid_Round*>::iterator it=rnd_vec.begin(); it !=rnd_vec.end(); ++it) 
		//	(*it)->update_continuos(playHeadPosition);
		//
		for (vector<Fid_Round*>::iterator it=rnd_vec.begin(); it !=rnd_vec.end(); ) { 			
			if( !(*it)->isAlive() ) {
				delete (*it);
				rnd_vec.erase(it);	
			} else {
				(*it)->update_continuos(playHeadPosition);
				it++;
			}
		}
				
		//for (vector<Fid_Rot*>::iterator it=rot_vec.begin(); it !=rot_vec.end(); ++it)
		//	(*it)->update_continuos(playHeadPosition);
		//
		for (vector<Fid_Rot*>::iterator it=rot_vec.begin(); it !=rot_vec.end(); ) { 			
			if( !(*it)->isAlive() ) {
				delete (*it);
				rot_vec.erase(it);	
			} else {
				(*it)->update_continuos(playHeadPosition);
				it++;
			}
		}
			
		//for (vector<Fid_Sqr*>::iterator it=sqr_vec.begin(); it !=sqr_vec.end(); ++it) 
		//	(*it)->update_continuos(playHeadPosition);
		//
		for (vector<Fid_Sqr*>::iterator it=sqr_vec.begin(); it !=sqr_vec.end(); ) { 			
			if( !(*it)->isAlive() ) {
				delete (*it);
				sqr_vec.erase(it);	
			} else {
				(*it)->update_continuos(playHeadPosition);
				it++;
			}
		}
		
		//for (vector<Fid_Synth*>::iterator it=synth_vec.begin(); it !=synth_vec.end(); ++it) 
		//	(*it)->update_continuos((float)time-last_croma_time);
		//
		for (vector<Fid_Synth*>::iterator it=synth_vec.begin(); it !=synth_vec.end(); ) { 			
			if( !(*it)->isAlive() ) {
				delete (*it);
				synth_vec.erase(it);	
			} else {
				(*it)->update_continuos((float)time-last_croma_time);
				it++;
			}
		}
		
		//for (vector<Fid_Chords*>::iterator it=chords_vec.begin(); it !=chords_vec.end(); ++it) 
		//	(*it)->update_continuos(playHeadPosition);
		//
		for (vector<Fid_Chords*>::iterator it=chords_vec.begin(); it !=chords_vec.end(); ) { 			
			if( !(*it)->isAlive() ) {
				delete (*it);
				chords_vec.erase(it);	
			} else {
				(*it)->update_continuos(playHeadPosition);
				it++;
			}
		}
		
		// per la classe 'Finger' utilizzo il vecchio metododi update 
		// la classe 'Finger' infatti non ha il metodo 'isAlive'
		for (vector<Finger*>::iterator it=dito_vec.begin(); it !=dito_vec.end(); ++it) 
			(*it)->update_continuos();
		
	
		// DISEGNO DELLA PLAYHEAD -----------------------------------------
		//ofDrawBitmapString("CROMA ORA IN RIPRODUZIONE: " + ofToString(n_crome-1), 20, 20);
		playHeadPosition = ofMap( (time-last_croma_time), 0, croma_time, 0, stripe_w);
	
		// PRIMA RESA GRAFICA
		// questa però non è molto piacevole perchè rispecchia perfettamente il funzionamento 
		// del programma: il suono è emesso non appena la playhead tocca la croma. 
		// Il problema è che spesso, il disegno del fiducial si trova
		// spazialmente all'interno della striscia che rappresenta la croma in esame.
		// per questo, sentire il suono ma non vedere ancora la playhead sovrapporsi al 
		// disegno del fiducial può mandare in confusione,
		// per questo, meglio la SECONDA RESA GRAFICA
		// playHeadPosition = playHeadPosition + ((n_crome-1)*stripe_w);
	
		// SECONDA RESA GRAFICA
		// aggiungo un offset pari a metà della larghezza della stripe
		// sì da avere la playhead che passa, mediamente, sopra al fiducial al momento in cui il suono viene emesso
		playHeadPosition = playHeadPosition + ((n_crome-1)*stripe_w) + stripe_w/2;
		playHeadPosition = playHeadPosition % wQuadro;

		digit.update();
	}
}

// DRAW ////////////////////////////////////////////////////////////////
void ofApp::draw() 
{
	
	if(bSetup) 
	{
		// se siamo in fase di setup iniziale 
		
		// COMINCIA IL DISEGNO ALL'INTERNO DELL'FBO //
		fbo.begin();
		ofEnableSmoothing();
		
		ofBackground(giallo);
	
		ofPushMatrix();
			ofTranslate(margine, margine, 0);
		
			ofPushStyle();
			ofSetColor(verde);
			ofDrawRectangle(quadro);
			ofPopStyle();
		
			stripe_w = wQuadro / NUM_STRIPES;
		
			// la striscia corrispondente diventa verde se vi trovo un fiducial quadrato
			for (vector<Fid_Sqr*>::iterator it=sqr_vec.begin(); it !=sqr_vec.end(); ++it) 
			{
				for(int i=0; i < NUM_STRIPES; ++i) 
				{
					if ( ((*it)->isAlive()) && ((*it)->getFidPos()->x >= (stripe_w*i)) && ((*it)->getFidPos()->x < (stripe_w*(i+1))) ) 
					{	
					ofPushStyle();
					ofEnableBlendMode(OF_BLENDMODE_ALPHA);
					ofSetColor(90, 200, 120, 100);
					ofFill();
					ofDrawRectangle(stripe_w*i, 0, stripe_w, hQuadro );
					ofDisableBlendMode();
					ofPopStyle();
					} 			
				}
			}
				
			// la striscia corrispondente diventa verde se vi trovo un fiducial rotondo
			for (vector<Fid_Round*>::iterator it=rnd_vec.begin(); it !=rnd_vec.end(); ++it) 
			{
				for(int i=0; i < NUM_STRIPES; ++i) 
				{
					if ( ((*it)->isAlive()) && ((*it)->getFidPos()->x >= (stripe_w*i)) && ((*it)->getFidPos()->x < (stripe_w*(i+1))) ) 
					{	
						ofPushStyle();
						ofEnableBlendMode(OF_BLENDMODE_ALPHA);
						ofSetColor(90, 200, 120, 100);
						ofFill();
						ofDrawRectangle(stripe_w*i, 0, stripe_w, hQuadro );
						ofDisableBlendMode();
						ofPopStyle();
					} 			
				}	
			}
		
		
			for(int i=0; i < NUM_STRIPES; ++i) 
			{
				ofPushStyle();
				ofSetHexColor(0x000000);
				ofNoFill();
				ofDrawRectangle(stripe_w*i, 0, stripe_w, hQuadro );
				ofPopStyle();
			}
		
			// DISEGNO DEI FIDUCIAL -------------------------------------------
			// chiamo il metodo DRAW per tutti i fiducial rotativi
			for (vector<Fid_Rot*>::iterator it=rot_vec.begin(); it !=rot_vec.end(); ++it) 
				(*it)->draw();
			// chiamo il metodo DRAW per tutti i fiducial quadrati
			for (vector<Fid_Sqr*>::iterator it=sqr_vec.begin(); it !=sqr_vec.end(); ++it) 		
				(*it)->draw();
			// chiamo il metodo DRAW per tutti i fiducial rotondi
			for (vector<Fid_Round*>::iterator it=rnd_vec.begin(); it !=rnd_vec.end(); ++it) 
				(*it)->draw();
			
			ofPushStyle();		
			ofSetHexColor(0x000000);
		
			// CALIBRATION INFO BOX POSITION
			// settare qui la posizione dell'angolo superiore sinitro del riquadro INFO
			// calibration info box
			int cibX	= wQuadro*0.5 + 20;
			int cibY	= hQuadro*0.5 - 280;
			int y = 0;
			ofDrawBitmapString("videoTavolo-v"+ofToString(vMajor)+"."+ofToString(vMinor)+"."+ofToString(vMaintenance), cibX, cibY + y);  y += 20;

			//ofDrawBitmapString("We are using PD via OSC (NO LibPD) !", cibX, cibY + y);  y += 20;
			ofDrawBitmapString("Elapsed Time: " + ofToString(hours) + " : " + ofToString(minutes) + " : " + ofToString(seconds), cibX, cibY + y );  y += 20;
			ofDrawBitmapString("OSC HOST: " + ofToString(HOST) + "; S: " + ofToString(S_PORT) + "; R: " + ofToString(R_PORT) + ";", cibX, cibY + y);  y += 20;
			ofDrawBitmapString("TUIO port: " + ofToString(TUIO_PORT),	cibX, cibY + y);  y += 20;
			ofDrawBitmapString("FPS:       " + ofToString(ofGetFrameRate()),	cibX, cibY + y);  y += 20;
			ofDrawBitmapString("Graphics transformation", cibX, cibY + y);	y += 20;
			ofDrawBitmapString("Rotations - X: " + ofToString(meshRotationX) + ", Y: " + ofToString(meshRotationY) + ", Z: " + ofToString(meshRotationZ) + ";",	cibX, cibY + y);  y += 20;
			ofDrawBitmapString("Mesh Center - X: " + ofToString(meshCenterX) + ", Y: " + ofToString(meshCenterY) + ", Z: " + ofToString(meshCenterZ) + ";", cibX, cibY + y); y += 20;
			
			// togli il commento sottostante se vuoi attivare
			// la possibilità di scalare e traslare le coordinate
			// reacTIVision raw.
			//ofDrawBitmapString("TUIO coords transformation", cibX, cibY + y);	y += 20;
			//ofDrawBitmapString("Translation - X: " +ofToString(rvTranslateX)+ ", Y: " +ofToString(rvTranslateY)+ ";", cibX, cibY + y); y += 20;
			//ofDrawBitmapString("Scale - X: " +ofToString(rvScaleX)+ ", Y: " +ofToString(rvScaleY)+ ";", cibX, cibY + y); y += 20;

			// CALIBRATION LEGENDA BOX POSITION
			// settare qui la posizione dell'angolo superiore sinitro del riquadro LEGENDA
			// calibration legenda box
			int clbX	= wQuadro*0.5 + 100;
			int clbY	= hQuadro*0.5 + 50;
			
			y = 0;
			ofDrawBitmapString("Graphics transformation", clbX, clbY + y);	y += 20;
			ofDrawBitmapString("ROTATE X [   Q - Z     ]", clbX, clbY + y); y += 20;
			ofDrawBitmapString("ROTATE Y [   W - X     ]", clbX, clbY + y); y += 20;
			ofDrawBitmapString("ROTATE Z [   E - C     ]", clbX, clbY + y); y += 20;
			ofDrawBitmapString("MOVE X   [LEFT - RIGHT ]", clbX, clbY + y); y += 20;
			ofDrawBitmapString("MOVE Y   [  UP - DOWN  ]", clbX, clbY + y); y += 20;
			ofDrawBitmapString("MOVE Z   [PGUP - PGDOWN]", clbX, clbY + y); y += 20;
			ofDrawBitmapString("RESET    [     R       ]", clbX, clbY + y); y += 20;
			
			// togli il commento sottostante se vuoi attivare
			// la possibilità di scalare e traslare le coordinate
			// reacTIVision raw.
			//ofDrawBitmapString("TUIO coords transformations:", clbX, clbY + y); y += 20;
			//ofDrawBitmapString("MOVE X   [   J - L     ]", clbX, clbY + y); y += 20;
			//ofDrawBitmapString("MOVE Y   [   I - K     ]", clbX, clbY + y); y += 20;
			//ofDrawBitmapString("SCALE X  [   T - B     ]", clbX, clbY + y); y += 20;
			//ofDrawBitmapString("SCALE Y  [   Y - N     ]", clbX, clbY + y); y += 20;
			//ofDrawBitmapString("RESET    [     M       ]", clbX, clbY + y); y += 20;
			
			ofPopStyle();
		
			// disegno della crociera centrale
			ofPushMatrix();
				ofTranslate(wQuadro*0.5, hQuadro*0.5);
				int lsegmento = 50;
				int spazio = 30;
				ofPushStyle();
				ofSetLineWidth(10);
				ofSetHexColor(0x000000);
		
				ofPushMatrix();
					ofTranslate(-lsegmento-spazio, -spazio);
					ofDrawLine(0, 0, lsegmento, 0);
				ofPopMatrix();
		
				ofPushMatrix();
					ofTranslate(-spazio, -spazio-lsegmento);
					ofDrawLine(0, 0, 0, lsegmento);
				ofPopMatrix();
		
				ofPushMatrix();
					ofTranslate(spazio, -spazio-lsegmento);
					ofDrawLine(0, 0, 0, lsegmento);
				ofPopMatrix();
		
				ofPushMatrix();
					ofTranslate(spazio, -spazio);
					ofDrawLine(0, 0, lsegmento, 0);
				ofPopMatrix();
		
				ofPushMatrix();
					ofTranslate(spazio, spazio);
					ofDrawLine(0, 0, lsegmento, 0);
				ofPopMatrix();
		
				ofPushMatrix();
					ofTranslate(spazio, spazio);
					ofDrawLine(0, 0, 0, lsegmento);
				ofPopMatrix();
		
				ofPushMatrix();
					ofTranslate(-spazio, spazio);
					ofDrawLine(0, 0, 0, lsegmento);
				ofPopMatrix();
		
				ofPushMatrix();
					ofTranslate(-spazio-lsegmento, spazio);
					ofDrawLine(0, 0, lsegmento, 0);
				ofPopMatrix();
		
			
			
		
			ofPopStyle();
		ofPopMatrix(); //ofTranslate(margine, margine, 0);

		// disegno degli angoli
		ofPushStyle();
		ofSetLineWidth(10);
		ofSetHexColor(0x000000);
		
		ofPushMatrix();
			ofTranslate(0 + spazio, 0 + spazio, 0);
			ofDrawLine(0, 0, lsegmento, 0);
			ofDrawLine(0, 0, 0, lsegmento);
		ofPopMatrix();
		
		ofPushMatrix();
			ofTranslate(wQuadro - spazio, 0 + spazio, 0);
			ofDrawLine(0, 0, -lsegmento, 0);
			ofDrawLine(0, 0, 0, lsegmento);
		ofPopMatrix();
		
		ofPushMatrix();
			ofTranslate(wQuadro - spazio, hQuadro - spazio, 0);
			ofDrawLine(0, 0, -lsegmento, 0);
			ofDrawLine(0, 0, 0, -lsegmento);
		ofPopMatrix();
		
		ofPushMatrix();
			ofTranslate(0 + spazio, hQuadro - spazio, 0);
			ofDrawLine(0, 0, lsegmento, 0);
			ofDrawLine(0, 0, 0, -lsegmento);
		ofPopMatrix();
		
		
		ofPopStyle();
		
		ofPopMatrix();
		fbo.end();
		
		
	} 
	else
	{
		// se invece stiamo utilizzando il programma in modo normale
		
		// COMINCIA IL DISEGNO ALL'INTERNO DELL'FBO //
		fbo.begin(); 
		ofEnableSmoothing();		
	
		// --- disegno del gradiente
		ofPushStyle();
		ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
		backgroundGradient(ofColor(255), ofColor(10), wFbo, hFbo);
		ofSetColor(color_sfondo);
		ofDrawRectangle(0, 0, wFbo, hFbo);
		ofDisableBlendMode();
		ofPopStyle();
		
		// --- disegno dell'immagine dei loghi
		ofPushMatrix();
		ofPopStyle();
		ofTranslate(margine, margine);
		ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
		newLogosImage.draw(0, 0);
		ofDisableBlendMode();
		ofPopStyle();
		ofPopMatrix();
		
		ofPushMatrix();
		ofTranslate(margine, margine);
		
		stripe_w = wQuadro / NUM_STRIPES;
	
		// se vi trovo un fiducial quadrato
		for (vector<Fid_Sqr*>::iterator it=sqr_vec.begin(); it !=sqr_vec.end(); ++it) 
		{
			for(int i=0; i < NUM_STRIPES; ++i) 
			{
				if ( ((*it)->isAlive()) && ((*it)->getFidPos()->x >= (stripe_w*i)) && ((*it)->getFidPos()->x < (stripe_w*(i+1))) ) 
				{	
					ofPushStyle();
					ofEnableBlendMode(OF_BLENDMODE_ALPHA);
					ofSetColor(90, 200, 120, 100);
					ofFill();
					// il primo e ultimo STRIPES devono essere larghi più delle stripes centrali
					if(i == 0)
						ofDrawRectangle(stripe_w*i-margine, 0-margine, stripe_w+margine, hFbo );
					else if (i == NUM_STRIPES -1)
						ofDrawRectangle(stripe_w*i, 0-margine, stripe_w+margine, hFbo );
					else 
						ofDrawRectangle(stripe_w*i, 0-margine, stripe_w, hFbo );
					
					ofDisableBlendMode();
					ofPopStyle();
				} 			
			}
		}
		
		// se vi trovo un fiducial rotondo
		for (vector<Fid_Round*>::iterator it=rnd_vec.begin(); it !=rnd_vec.end(); ++it) 
		{
			for(int i=0; i < NUM_STRIPES; ++i) 
			{
				if ( ((*it)->isAlive()) && ((*it)->getFidPos()->x >= (stripe_w*i)) && ((*it)->getFidPos()->x < (stripe_w*(i+1))) ) 
				{	
					ofPushStyle();
					ofEnableBlendMode(OF_BLENDMODE_ALPHA);
					ofSetColor(90, 200, 120, 100);
					ofFill();
					// il primo e ultimo STRIPES devono essere larghi più delle stripes centrali
					if(i == 0)
						ofDrawRectangle(stripe_w*i-margine, 0-margine, stripe_w+margine, hFbo );
					else if (i == NUM_STRIPES -1)
						ofDrawRectangle(stripe_w*i, 0-margine, stripe_w+margine, hFbo );
					else 
						ofDrawRectangle(stripe_w*i, 0-margine, stripe_w, hFbo );
					
					ofDisableBlendMode();
					ofPopStyle();
				} 			
			}
		}
	
	
		for(int i=1; i < NUM_STRIPES-1; ++i) // disegnamo i contorni delle sole stripes centrali
		{
			ofPushStyle();
			ofSetHexColor(0x000000);
			ofNoFill();
			ofDrawRectangle(stripe_w*i, 0-margine, stripe_w, hFbo );
			ofPopStyle();
		}
	

		// DISEGNO DELLA PLAYHEAD -----------------------------------------
		// striscie
		
		//cout << playHeadPosition << "\n";
		for(int i=0; i < NCOLUMNS; ++i) {
			striscie[i].checka(playHeadPosition);
		}	
		//playhead
		ofPushStyle();
		ofSetLineWidth(5); 
		ofSetColor(color_playhead);
		ofDrawLine(playHeadPosition, 0-margine, playHeadPosition, hFbo);
		ofPopStyle();
	
	
		// DISEGNO DEI FIDUCIAL -------------------------------------------
		// chiamo il metodo DRAW per tutti i fiducial BASSO
		for (vector<Fid_Bass*>::iterator it=bass_vec.begin(); it !=bass_vec.end(); ++it) 
			(*it)->draw();
		// chiamo il metodo DRAW per tutti i fiducial CHORDS
		for (vector<Fid_Chords*>::iterator it=chords_vec.begin(); it !=chords_vec.end(); ++it) 
			(*it)->draw();
		// chiamo il metodo DRAW per tutti i fiducial SYNTH
		for (vector<Fid_Synth*>::iterator it=synth_vec.begin(); it !=synth_vec.end(); ++it) 
			(*it)->draw();
		// chiamo il metodo DRAW per tutti i fiducial rotativi (BPM)
		for (vector<Fid_Rot*>::iterator it=rot_vec.begin(); it !=rot_vec.end(); ++it) 
			(*it)->draw();
		// chiamo il metodo DRAW per tutti i fiducial quadrati (SNARE + HIHAT)
		for (vector<Fid_Sqr*>::iterator it=sqr_vec.begin(); it !=sqr_vec.end(); ++it) 		
			(*it)->draw();
		// chiamo il metodo DRAW per tutti i fiducial rotondi (KICK)
		for (vector<Fid_Round*>::iterator it=rnd_vec.begin(); it !=rnd_vec.end(); ++it) 
			(*it)->draw();
		// chiamo il metodo DRAW per tutti i finger (FINGER)
		for (vector<Finger*>::iterator it=dito_vec.begin(); it !=dito_vec.end(); ++it) 
			(*it)->draw();
		
		// DISEGNO LE INFO DEL DEBUG --------------------------------------
		// se debuggo, chiamo il metodo DEBUG per tutti i fiducial
		if(bDebug)
		{
			
			for (vector<Fid_Bass*>::iterator it=bass_vec.begin(); it !=bass_vec.end(); ++it) 
				(*it)->debug();
			
			for (vector<Fid_Chords*>::iterator it=chords_vec.begin(); it !=chords_vec.end(); ++it) 
				(*it)->debug();
			
			for (vector<Fid_Synth*>::iterator it=synth_vec.begin(); it !=synth_vec.end(); ++it) 
				(*it)->debug();
			
			for (vector<Fid_Rot*>::iterator it=rot_vec.begin(); it !=rot_vec.end(); ++it) 
				(*it)->debug();
		
			for (vector<Fid_Sqr*>::iterator it=sqr_vec.begin(); it !=sqr_vec.end(); ++it) 
				(*it)->debug();
			
			for (vector<Finger*>::iterator it=dito_vec.begin(); it !=dito_vec.end(); ++it) 
				(*it)->debug();
			
			for (vector<Fid_Round*>::iterator it=rnd_vec.begin(); it !=rnd_vec.end(); ++it) 
				(*it)->debug();
			
			
			// centro del tavolo
			ofPushMatrix();
			ofTranslate(wQuadro*0.5, hQuadro*0.5);

			ofPushStyle();
			ofSetHexColor(0xFF0000);
			ofSetLineWidth(4); 
			ofDrawLine( -20, 0, 20, 0);
			ofDrawLine(0, -20, 0, 20);
			ofPopStyle();

			ofPushMatrix();
			ofPushStyle();
			ofTranslate(5, -5);
			ofSetHexColor(0x000000);
			ofDrawBitmapString("CENTER", 0, 0); 	
			ofDrawBitmapString("FPS: " + ofToString((int)ofGetFrameRate()), 0, 13);
			ofDrawBitmapString("Elapsed Time: " + ofToString(hours) + " : " + ofToString(minutes) + " : " + ofToString(seconds), 0, 26 );
			ofDrawBitmapString("Bpm: " + ofToString(bpm), 0, 39);
			ofDrawBitmapString(ofToString(rot_vec.size())	 + " fid rot (Bpm)", 0, 52);
			ofDrawBitmapString(ofToString(rnd_vec.size())	 + " fid rnd (Kick)", 0, 65);
			ofDrawBitmapString(ofToString(sqr_vec.size())	 + " fid sqr (Snare, HiHat)", 0, 78);
			ofDrawBitmapString(ofToString(bass_vec.size())   + " fid bass", 0, 91);
			ofDrawBitmapString(ofToString(chords_vec.size()) + " fid chords", 0, 104);
			ofDrawBitmapString(ofToString(synth_vec.size())	 + " fid synth", 0, 117);
			//ofDrawBitmapString(ofToString(dito_vec.size()) + " fingers", 0, 117);
			
			ofPopStyle();
			ofPopMatrix();

			ofPopStyle();
			ofPopMatrix();
		}
	 
		digit.draw(wQuadro);		

		ofDisableSmoothing();
		ofPopMatrix();
		fbo.end(); // fine del disegno su FBO
	
	}

	
	// CARICO I DATI NELLA TEXTURE //
	text = fbo.getTextureReference();
	
	// DISEGNO LA MESH CON LA TEXTURE BINDATA //
	ofPushStyle();
	
	// disegno lo sfondo
	if(bSetup) 
	{
		ofBackground(giallo);
	} 
	else 
	{
		ofBackground(0, 0, 0);
	}
		
	ofPushMatrix();
	
	ofTranslate(wWindow*0.5, hWindow*0.5, 0);

	ofRotateX( meshRotationX ); 
    ofRotateY( meshRotationY );
    ofRotateZ( meshRotationZ );
        
    ofTranslate(meshCenterX, meshCenterY, meshCenterZ);	
	//ofTranslate(0, -wMesh*0.5+margine, 0);
	
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	
	text.bind(); // bind the image to begin texture mapping
	//mesh.drawWireframe();
	mesh.draw();
	text.unbind();
	
	ofDisableBlendMode();
	
	ofPopMatrix();
	
	ofPopStyle();
	
	// DA ELIMINARE INFINE
	// a little bit of text to try to understand something
    ofPushStyle();
    ofSetColor( black );
    if( ofGetWindowMode() )
        ofDrawBitmapString("FULLSCREEN MODE", 10, 15);
    else
        ofDrawBitmapString("WINDOW MODE", 10, 10);
    ofDrawBitmapString("W: "+ ofToString( ofGetWindowWidth() ) + "; H: "+ ofToString( ofGetWindowHeight() ) +";", 10, 30);
    ofPopStyle();
}

// EXIT ////////////////////////////////////////////////////////////////
void ofApp::exit() 
{
	cout << "OF: chiusura dell'applicativo\nSi procede con le routine di chiusura\n" << endl;
	
// #ifdef _LIBPD
// 	core.send_float("pads", 0);
// 	core.send_float("bass", 0);
// 	core.exit();
// #else
	// implementare un sistema per annullare i suoni che eventualemte 
	// dovessero essere ancora in fase di riproduzione al momento della 
	// chiusura del programma
	
	ofxOscMessage m;
	m.setAddress("/pads");
	m.addIntArg(0);
	sender.sendMessage(m);
	m.clear();
	m.setAddress("/bass");
	m.addIntArg(0);
	sender.sendMessage(m);
	m.clear();
// #endif
	
	vector<Fid_Rot*>::iterator it1=rot_vec.begin();
	cout << "ROT_LIST: " << rot_vec.size() << " puntatori ancora presenti nel vettore\n";
	while( !rot_vec.empty() && it1!=rot_vec.end() ) {
		delete *it1;
		//(*it1)->removed();
		++it1;
		cout << "\tFID_ROT: elemento rimosso!\n";
	}
	rot_vec.clear();
	cout << "ROT_LIST: lista ripulita\n" << endl;
	
	
	vector<Fid_Sqr*>::iterator it2=sqr_vec.begin();
	cout << "SQR_LIST: " << sqr_vec.size() << " puntatori ancora presenti nel vettore\n";
	while( !sqr_vec.empty() && it2!=sqr_vec.end() ) {
		delete *it2;
		//(*it2)->removed();
		++it2;
		cout << "\tFID_SQR: elemento rimosso!\n";
	}
	sqr_vec.clear();
	cout << "SQR_LIST: lista ripulita\n" << endl;
	
	
	vector<Finger*>::iterator it3=dito_vec.begin();
	cout << "DITO_LIST: " << dito_vec.size() << " puntatori ancora presenti nel vettore\n";
	while( !dito_vec.empty() && it3!=dito_vec.end() ) {
		delete *it3;
		//(*it3)->removed();
		++it3;
		cout << "\tFINGER: elemento rimosso!\n";
	}
	dito_vec.clear();
	cout << "DITO_LIST: lista ripulita\n" << endl;
	
	
	vector<Fid_Round*>::iterator it4=rnd_vec.begin();
	cout << "RND_LIST: " << rnd_vec.size() << " puntatori ancora presenti nel vettore\n";
	while( !rnd_vec.empty() && it4!=rnd_vec.end() ) {
		delete *it4;
		//(*it4)->removed();
		++it4;
		cout << "\tFID_ROUND: elemento rimosso!\n";
	}
	rnd_vec.clear();
	cout << "RND_LIST: lista ripulita\n" << endl;
	
	
	vector<Fid_Synth*>::iterator it5=synth_vec.begin();
	cout << "SINT_LIST: " << synth_vec.size() << " puntatori ancora presenti nel vettore\n";
	while( !synth_vec.empty() && it5!=synth_vec.end() ) {
		delete *it5;
		//(*it5)->removed();
		++it5;
		cout << "\tFID_SYNTH: elemento rimosso!\n";
	}
	synth_vec.clear();
	cout << "SINT_LIST: lista ripulita\n" << endl;
	
	
	vector<Fid_Chords*>::iterator it6=chords_vec.begin();
	cout << "CHORDS_LIST: " << chords_vec.size() << " puntatori ancora presenti nel vettore\n";
	while( !chords_vec.empty() && it6!=chords_vec.end() ) {
		delete *it6;
		//(*it6)->removed();
		++it6;
		cout << "\tFID_CHORDS: elemento rimosso!\n";
	}
	chords_vec.clear();
	cout << "CHORDS_LIST: lista ripulita\n" << endl;
	
	// gli oggetti creati senza utilizzare l'operatore 'new' vengono automaticamente distrutti
	// non appena usciamo dal programma (uscendo dallo scope).
	//delete matrice;
	//delete digit;
	//for(int i = 0; i < NCOLUMNS; ++i) {
	//	delete striscie[i];
	//}
	
}

// ALTRI METODI ////////////////////////////////////////////////////////
// ogni volta che premo un determinato pulsante sulla tastiera del computer
// questo si traduce in un comportamento del programma
void ofApp::keyPressed(int key) 
{
	switch (key) 
	{
		case 'd':
		case 'D':
		  if(!bSetup)
			bDebug = !bDebug;
		  break;
			
		case 'f':
		case 'F':
		  ofToggleFullscreen();
		  break;
			
		case 'h':
		case 'H':
		  if(!bDebug)
			bSetup = !bSetup;
		  break;
			
        // GRAFICA: rotazioni
		case 'q':
        case 'Q':
		  if(bSetup)
			meshRotationX -= 0.25;
		  break;
		case 'z':
		case 'Z':
		  if(bSetup)
			meshRotationX += 0.25;
		  break;

		case 'w':
        case 'W':
		  if(bSetup)
			meshRotationY -= 0.25;
		  break;
		case 'x':
		case 'X':
		  if(bSetup)
            meshRotationY += 0.25;
		  break;

		case 'e':
        case 'E':
		  if(bSetup)
			meshRotationZ -= 0.25;
		  break;
		case 'c':
		case 'C':
		  if(bSetup)
            meshRotationZ += 0.25;
		  break;
			
		// GRAFICA: traslazioni
		case OF_KEY_LEFT:
		  if(bSetup)
			meshCenterX -= 0.5;
		  break;
			
		case OF_KEY_RIGHT:
		  if(bSetup)
			meshCenterX += 0.5;
		  break;

        case OF_KEY_UP:
		  if(bSetup)
			meshCenterY -= 0.5;
		  break;
			
		case OF_KEY_DOWN:
		  if(bSetup)
			meshCenterY += 0.5;
		  break;
        
        case OF_KEY_PAGE_UP:
		  if(bSetup)
			meshCenterZ -= 0.5;
		  break;
			
		case OF_KEY_PAGE_DOWN:
		  if(bSetup)
			meshCenterZ += 0.5;
		  break;
		
		/*
		// GRAFICA: reset
		case 'r':
		case 'R':
		  if(bSetup)
			meshReset();
		  break;
		 
		// reacTIVision coords: translate
		case 'i':
		case 'I':
		  rvTranslateY += 0.0025;
		  break;
		  
		case 'k':
		case 'K':
		  rvTranslateY -= 0.0025;
		  break;
		  
		case 'j':
		case 'J':
		  rvTranslateX -= 0.0025;
		  break;
		  
		case 'l':
		case 'L':
		  rvTranslateX += 0.0025;
		  break;
		  
		// reacTIVision coords: scale
		case 't':
		case 'T':
		  rvScaleX += 0.0025;
		  break;
		
		case 'b':
		case 'B':
		  rvScaleX -= 0.0025;
		  break;
		  
		case 'y':
		case 'Y':
		  rvScaleY += 0.0025;
		  break;
		
		case 'n':
		case 'N':
		  rvScaleY -= 0.0025;
		  break;
		
		// reacTIVision coords: reset
		case 'm':
		case 'M':
		  rvReset();
		  break;
		*/
		
		default:
		  cout << "No valid key pressed\n" << endl;
	}
}

//----------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
  if( ofGetWindowMode() ) 
    std::cout << "Window Mode = FULLSCREEN\n";
  else
    std::cout << "Window Mode = WINDOW\n" ;	
  wWindow = ofGetWindowWidth();
  hWindow = ofGetWindowHeight();
  std::cout << wWindow << ", " << hWindow << ";\n";
}

float ofApp::TUIOscaleX( float TUIOx )
{
  	float scaled = TUIOx;
	
	// Now we calculate the fiducial position to be able to draw it on screen.
	// Remember: reacTIVision coordinates (that are sent to OF via TUIO protocol)
	// are 0-1 normalized according to the reacTIVision calibrated grid dimensions.
	// However our game surface covers only a part of the entire grid in a way that 
	// we only have to consider objects of X coordinates between 0.125 and 0.875.
	//
	//  0  0.125            0.875 1
	//	/-- I==|==|==|==|==|==I --\
	//	|-- I--|--|--|--|--|--I --|
	//	|-- I--|--|--|--|--|--I --|
	//	|-- I--|--|--|--|--|--I --|
	//	|-- I--|--|--|--|--|--I --|
	//	|-- I--|--|--|--|--|--I --|
	//	\-- I==|==|==|==|==|==I --/
	//
	// so, to correctly render the object on screen, we use a mathematical expression:
	// (tuioObject.getX() - 0.125 )/0.75 ---> (8*tuioObject.getX() - 1)/6
	
	scaled = (8*scaled - 1) / 6; 
	
	// togli il commento sottostante se vuoi attivare
	// la possibilità di scalare e traslare le coordinate
	// reacTIVision raw.
	//scaled += rvTranslateX;
	//scaled = (scaled - 0.5) * rvScaleX + 0.5;
	
	return scaled;  
}

float ofApp::TUIOscaleY( float TUIOy )
{
  	float scaled  = TUIOy;
	
	// togli il commento sottostante se vuoi attivare
	// la possibilità di scalare e traslare le coordinate
	// reacTIVision raw.
	//scaled += rvTranslateY;
	//scaled = (scaled - 0.5) * rvScaleY + 0.5;
	
	return scaled; 
}

// OBJECT ADDED ////////////////////////////////////////////////////////
// ogni volta che una oggetto viene aggiunto sulla superficie di gioco
void ofApp::objectAdded(ofxTuioObject & tuioObject)
{	
	int posX;		// variabile di appoggio per identificare la posizione relativa del fiducial dentro alla matrice 
	float xScaled;	// variabile di supporto per mappare la posizione TUIO nel range utilizzato dal nostro applicativo
	float yScaled;
	int session_id	= tuioObject.getSessionId();
	int fiducial_id = tuioObject.getFiducialId();
	int piano;
	
	angolo = tuioObject.getAngle() * (-1);			// passo anche il (-1) perchè da reactivision, l'angolo è invertito
	rot_vel = tuioObject.getRotationSpeed() * (-1);	// passo anche il (-1) perchè da reactivision, l'angolo è invertito
	
	xScaled = TUIOscaleX( tuioObject.getX() );
	yScaled = TUIOscaleY( tuioObject.getY() );
	pos.x = wQuadro * xScaled;		
	pos.y = hQuadro * yScaled;
	
	switch(fiducial_id)
	{
		 
		case KICK:
		{

			posX = xScaled * 8; //in che croma siamo?
			piano=1;
			matrice.add_to_table(posX, piano, session_id);
			
			rotondo = new Fid_Round(fiducial_id, session_id);
			rnd_vec.push_back(rotondo);
			rnd_vec.back()->setup(&pos, &centro, angolo, color_kick);
			rnd_vec.back()->added();
			
			break;
		}
		case SNARE:
		{
			
			posX = xScaled * 8;
			piano=2;
			matrice.add_to_table(posX, piano, session_id);
			
			quadrato = new Fid_Sqr(fiducial_id, session_id);
			sqr_vec.push_back(quadrato);
			sqr_vec.back()->setup(&pos, &centro, angolo, color_snare);
			sqr_vec.back()->added();
			
			break;
		}
		case HIHAT: 
		{
			posX = xScaled * 8;
			piano=3;
			matrice.add_to_table(posX, piano, session_id);
			
			quadrato = new Fid_Sqr(fiducial_id, session_id);
			sqr_vec.push_back(quadrato);
			sqr_vec.back()->setup(&pos, &centro, angolo, color_hihat);
			sqr_vec.back()->added();

			break;
		}
		case BPM: 
		{			
			
			rotativo = new Fid_Rot(fiducial_id, session_id);
			rot_vec.push_back(rotativo);
			rot_vec.back()->setup(&pos, &centro, angolo, memoria_bpm_angle, color_bpm, color_bpm_corona);
			rot_vec.back()->added();
			rot_vec.back()->update_interrupt(&pos, &centro, angolo, rot_vel); 
			// aggiorno la memoria dell'angolo per poterlo riutilizzare la prossima creazione di uno setsso tipo d'oggetto
			memoria_bpm_angle = rot_vec.back()->get_lim_angle(); 
			digit.set_bpm(bpm); // mostro i digit non appena posiziono il Fid_Rod per avere un riscontro immediato
			break;

		}
		case BASS:
		{
			ofxOscMessage m;
			m.setAddress("/bass");
			m.addIntArg(1);
			sender.sendMessage(m);
			
			//posX = tuioObject.getX() * 8; //in che croma siamo?
			//piano=1;
			//matrice.add_to_table(posX, piano, session_id);
			
			basso = new Fid_Bass(fiducial_id, session_id);
			bass_vec.push_back(basso);
			bass_vec.back()->setup(&pos, &centro, angolo, color_bass);
			bass_vec.back()->added();
			
			break;			
		}
		case PADS:
		{
			ofxOscMessage m;
			m.setAddress("/pads");
			m.addIntArg(1);
			sender.sendMessage(m);
			
			//posX = tuioObject.getX() * 8;
			//piano=3;
			//matrice.add_to_table(posX, piano, session_id);
			
			synth = new Fid_Synth(fiducial_id, session_id);
			synth_vec.push_back(synth);
			synth_vec.back()->setup(&pos, &centro, angolo, color_pads);
			synth_vec.back()->added();
			// setto il valore per 'f', 'T' e 't0' (e' necessario sottrarre 1 al valore della croma per non avere,
			// nell'animazione del synth un effetto 'scattoso' nella parte iniziale dell'animazione )
			synth_vec.back()->reset_internal_timer(n_crome-1);
			//synth_vec.back()->update_continuos((float)time-last_croma_time); // setto il valore per 't'
			
			break;
		}
		case CHORD_0:
		{
			chord_number=0;
			
			ofxOscMessage m;
			m.setAddress("/chord_num");
			m.addIntArg(chord_number);
			sender.sendMessage(m);
			
			accordo = new Fid_Chords(fiducial_id, session_id);
			chords_vec.push_back(accordo);
			chords_vec.back()->setup(&pos, &centro, angolo, color_accordo_0);
			chords_vec.back()->added();
			chords_vec.back()->update_interrupt(&pos, &centro, angolo, rot_vel); //commentarlo?

			
			break;
		}
		case CHORD_1:
		{
			chord_number=1;
			
			ofxOscMessage m;
			m.setAddress("/chord_num");
			m.addIntArg(chord_number);
			sender.sendMessage(m);
			
			accordo = new Fid_Chords(fiducial_id, session_id);
			chords_vec.push_back(accordo);
			chords_vec.back()->setup(&pos, &centro, angolo, color_accordo_1);
			chords_vec.back()->added();
			chords_vec.back()->update_interrupt(&pos, &centro, angolo, rot_vel);
			
			break;
		}
		case CHORD_2:
		{
			chord_number=2;
			
			ofxOscMessage m;
			m.setAddress("/chord_num");
			m.addIntArg(chord_number);
			sender.sendMessage(m);
			
			accordo = new Fid_Chords(fiducial_id, session_id);
			chords_vec.push_back(accordo);
			chords_vec.back()->setup(&pos, &centro, angolo, color_accordo_2);
			chords_vec.back()->added();
			chords_vec.back()->update_interrupt(&pos, &centro, angolo, rot_vel);
			
			break;
		}
		case CHORD_3:
		{
			chord_number=3;
			
			ofxOscMessage m;
			m.setAddress("/chord_num");
			m.addIntArg(chord_number);
			sender.sendMessage(m);
			
			accordo = new Fid_Chords(fiducial_id, session_id);
			chords_vec.push_back(accordo);
			chords_vec.back()->setup(&pos, &centro, angolo, color_accordo_3);
			chords_vec.back()->added();
			chords_vec.back()->update_interrupt(&pos, &centro, angolo, rot_vel);
			
			break;
		}
		case CHORD_4:
		{
			chord_number=4;
			
			ofxOscMessage m;
			m.setAddress("/chord_num");
			m.addIntArg(chord_number);
			sender.sendMessage(m);
			
			accordo = new Fid_Chords(fiducial_id, session_id);
			chords_vec.push_back(accordo);
			chords_vec.back()->setup(&pos, &centro, angolo, color_accordo_4);
			chords_vec.back()->added();
			chords_vec.back()->update_interrupt(&pos, &centro, angolo, rot_vel);
			
			break;
		}
		case CHORD_5:
		{
			chord_number=5;
			
			ofxOscMessage m;
			m.setAddress("/chord_num");
			m.addIntArg(chord_number);
			sender.sendMessage(m);
			
			accordo = new Fid_Chords(fiducial_id, session_id);
			chords_vec.push_back(accordo);
			chords_vec.back()->setup(&pos, &centro, angolo, color_accordo_5);
			chords_vec.back()->added();
			chords_vec.back()->update_interrupt(&pos, &centro, angolo, rot_vel);
			
			break;
		}
		default:
		{
			cout << "Fiducial ID non valido." << endl;
			break;
		}
	}
		
}

// OBJECT UPDATED //////////////////////////////////////////////////////
// ogni volta che una oggetto viene modificato sulla superficie di gioco
void ofApp::objectUpdated(ofxTuioObject & tuioObject)
{	
	int posX;
	float xScaled;	// variabile di supporto per mappare la posizione TUIO nel range utilizzato dal nostro applicativo
	float yScaled;
	int session_id	= tuioObject.getSessionId();
	int fiducial_id	= tuioObject.getFiducialId();
	int piano;

	angolo = tuioObject.getAngle() * (-1);			// passo anche il (-1) perchè da reactivision, l'angolo è invertito
	rot_vel = tuioObject.getRotationSpeed() * (-1);	// passo anche il (-1) perchè da reactivision, l'angolo è invertito
	
	// see 'objectAdded' if you want to know why we are doing this math 
	
	xScaled = TUIOscaleX( tuioObject.getX() );
	yScaled = TUIOscaleY( tuioObject.getY() );
	pos.x = wQuadro * xScaled;		
	pos.y = hQuadro * yScaled;
	
	switch(fiducial_id)
	{
		case KICK:
		{
			posX = xScaled * 8;
			piano=1;
			matrice.update_table(posX, piano, session_id);
			
			for (vector<Fid_Round*>::iterator it=rnd_vec.begin(); it !=rnd_vec.end(); ++it) 
			{
				if ( (*it)->get_s_id() == session_id) 
				{
					(*it)->update_interrupt(&pos, &centro, angolo, rot_vel);	
					break;
				}
				
			}	
			break;
		}
		case SNARE:
		{
			posX = xScaled * 8;
			piano=2;
			matrice.update_table(posX, piano, session_id);
	
			for (vector<Fid_Sqr*>::iterator it=sqr_vec.begin(); it !=sqr_vec.end(); ++it) 
			{
				if ( (*it)->get_s_id() == session_id) 
				{
					(*it)->update_interrupt(&pos, &centro, angolo, rot_vel);	
					break;
				}
				
			}
			break;
		}
		case HIHAT:
		{
			posX = xScaled * 8;
			piano=3;
			matrice.update_table(posX, piano, session_id);

			for (vector<Fid_Sqr*>::iterator it=sqr_vec.begin(); it !=sqr_vec.end(); ++it) 
			{
				if ( (*it)->get_s_id() == session_id) 
				{
					(*it)->update_interrupt(&pos, &centro, angolo, rot_vel);	
					break;
				}
			}
			break;
		}
		case BPM:
		{

			for (vector<Fid_Rot*>::iterator it=rot_vec.begin(); it !=rot_vec.end(); ++it) 
			{
				if ( (*it)->get_s_id() == session_id) 
				{
					(*it)->update_interrupt(&pos, &centro, angolo, rot_vel);
					// aggiorno la mamoria dell'angolo per poterlo riutilizzare la porssima creazione di uno stsso tipo d'oggetto
					memoria_bpm_angle = rot_vec.back()->get_lim_angle(); 
					
					break;
				}
				
			}
			
			bpm = ofMap(memoria_bpm_angle, -FIDUCIAL_MER, FIDUCIAL_MER, 30, 260, true);
			//Fid_Synth::synth_bpm = bpm; //questo metodo è stato sostituito da un sistema più efficiente in ofApp::update;
			digit.set_bpm(bpm);
			break;

		}
		case BASS:
		{
			//posX = tuioObject.getX() * 8;
			//piano=1;
			//matrice.update_table(posX, piano, session_id);
			
			for (vector<Fid_Bass*>::iterator it=bass_vec.begin(); it !=bass_vec.end(); ++it) 
			{
				if ( (*it)->get_s_id() == session_id) 
				{
					(*it)->update_interrupt(&pos, &centro, angolo, rot_vel);	
					break;
				}
				
			}	
			break;
		}
		case CHORD_0:
		case CHORD_1:
		case CHORD_2:
		case CHORD_3:
		case CHORD_4:
		case CHORD_5:
		{
			
			for (vector<Fid_Chords*>::iterator it=chords_vec.begin(); it !=chords_vec.end(); ++it) 
			{
				if ( (*it)->get_s_id() == session_id) 
				{
					(*it)->update_interrupt(&pos, &centro, angolo, rot_vel);
					// aggiorno la mamoria dell'angolo per poterlo riutilizzare la porssima creazione di uno stsso tipo d'oggetto
					//memoria_bpm_angle = rot_vec.back()->get_lim_angle(); 
					
					break;
				}
				
			}
			
			//bpm = ofMap(memoria_bpm_angle, -FIDUCIAL_MER, FIDUCIAL_MER, 30, 260, true);
			//digit.set_bpm(bpm);
			break;
			
		}
		case PADS:
		{
			//posX = tuioObject.getX() * 8;
			//piano=2;
			//matrice.update_table(posX, piano, session_id);
			
			for (vector<Fid_Synth*>::iterator it=synth_vec.begin(); it !=synth_vec.end(); ++it) 
			{
				if ( (*it)->get_s_id() == session_id) 
				{
					(*it)->update_interrupt(&pos, &centro, angolo, rot_vel);	
					break;
				}
				
			}
			break;
		}
		default:
		{
			cout << "Fiducial ID non valido." << endl;
			break;
			
		}
	}
}

// OBJECT REMOVED //////////////////////////////////////////////////////
// ogni volta che una oggetto viene rimosso dalla superficie di gioco
void ofApp::objectRemoved(ofxTuioObject & tuioObject)
{
	int session_id	= tuioObject.getSessionId();
	int fiducial_id	= tuioObject.getFiducialId();
	int piano;
	
	switch(fiducial_id)
	{
		case KICK:
		{
			piano=1;
			// forse è superfluo passare il "piano" alla funzione "matrice.remove_from_table"
			matrice.remove_from_table(piano, session_id);
			
			vector<Fid_Round*>::iterator it;
			for (it=rnd_vec.begin(); it !=rnd_vec.end(); ++it) 
			{
				if ( (*it)->get_s_id() == session_id) 
				{
					(*it)->removed();
					break;
				}
			}
			//rnd_vec.erase(it);
			
			
			break;
		}
		case SNARE:
		{
			piano=2;
			matrice.remove_from_table(piano, session_id);
			
			vector<Fid_Sqr*>::iterator it;
			for (it=sqr_vec.begin(); it !=sqr_vec.end(); ++it) 
			{
				if ( (*it)->get_s_id() == session_id) 
				{
					(*it)->removed();
					break;
				}
			}
			//sqr_vec.erase(it);
			break;
		}
		case HIHAT:
		{
			piano=3;
			matrice.remove_from_table(piano, session_id);
			
			vector<Fid_Sqr*>::iterator it;
			for (it=sqr_vec.begin(); it !=sqr_vec.end(); ++it) 
			{
				if ( (*it)->get_s_id() == session_id) 
				{
					(*it)->removed();
					break;
				}
			}
			//sqr_vec.erase(it);
			break;
		}
		case BPM:
		{
			
			vector<Fid_Rot*>::iterator it;
			for (it=rot_vec.begin(); it !=rot_vec.end(); ++it) 
			{
				if ( (*it)->get_s_id() == session_id) 
				{
					(*it)->removed();
					break;
				}
			}
			//rot_vec.erase(it);
			
			break;
		}
		case BASS:
		{

			ofxOscMessage m;
			m.setAddress("/bass");
			m.addIntArg(0);
			sender.sendMessage(m);
			
			//piano=1;
			// forse è superfluo passare il "piano" alla funzione "matrice.remove_from_table"
			//matrice.remove_from_table(piano, session_id);
			
			vector<Fid_Bass*>::iterator it;
			for (it=bass_vec.begin(); it !=bass_vec.end(); ++it) 
			{
				if ( (*it)->get_s_id() == session_id) 
				{
					(*it)->removed();
					break;
				}
			}
			//bass_vec.erase(it);
			
			break;
			
		}
		case PADS:
		{
			
			ofxOscMessage m;
			m.setAddress("/pads");
			m.addIntArg(0);
			sender.sendMessage(m);
			
			//piano=3;
			//matrice.remove_from_table(piano, session_id);
			
			vector<Fid_Synth*>::iterator it;
			for (it=synth_vec.begin(); it !=synth_vec.end(); ++it) 
			{
				if ( (*it)->get_s_id() == session_id) 
				{
					(*it)->removed();
					break;
				}
			}
			//synth_vec.erase(it);
			
			break;
		}
		case CHORD_0:
		case CHORD_1:
		case CHORD_2:
		case CHORD_3:
		case CHORD_4:
		case CHORD_5:
		{
			
			vector<Fid_Chords*>::iterator it;
			for (it=chords_vec.begin(); it !=chords_vec.end(); ++it) 
			{
				if ( (*it)->get_s_id() == session_id) 
				{
					(*it)->removed();
					break;
				}
			}
			//chords_vec.erase(it);
			
			break;
		}
		default:
		{
			cout << "Fiducial ID non valido." << endl;
			break;
		}
	}
	
	
}

// FINGER ADDED ////////////////////////////////////////////////////////
void ofApp::cursorAdded(ofxTuioCursor & tuioCursor)
{
	int finger_id = tuioCursor.getFingerId();
	float xScaled, yScaled;
	
	xScaled = TUIOscaleX( tuioCursor.getX() );
	yScaled = TUIOscaleY( tuioCursor.getY() );
	pos.x = wQuadro * xScaled;		
	pos.y = hQuadro * yScaled;
	
	dito = new Finger(finger_id);
	dito_vec.push_back(dito);
	dito_vec.back()->setup(&pos, blue);
	dito_vec.back()->added();	
}

// FINGER UPDATED //////////////////////////////////////////////////////
void ofApp::cursorUpdated(ofxTuioCursor & tuioCursor)
{
	int finger_id = tuioCursor.getFingerId();
	
	float xScaled, yScaled;
	
	xScaled = TUIOscaleX( tuioCursor.getX() );
	yScaled = TUIOscaleY( tuioCursor.getY() );
	pos.x = wQuadro * xScaled;		
	pos.y = hQuadro * yScaled;
	
	int x_speed = tuioCursor.getXSpeed();
	int y_speed = tuioCursor.getYSpeed();
	int motion_speed = tuioCursor.getMotionSpeed();
	int motion_accel = tuioCursor.getMotionAccel();
	
	for (vector<Finger*>::iterator it=dito_vec.begin(); it !=dito_vec.end(); ++it) 
	{
		if ( (*it)->get_finger_id() == finger_id) 
		{
			(*it)->update_interrupt(&pos);	
			break;
		}
	}
}

// FINGER REMOVED //////////////////////////////////////////////////////
void ofApp::cursorRemoved(ofxTuioCursor & tuioCursor)
{
	int finger_id = tuioCursor.getFingerId();
	pos.x = wQuadro * tuioCursor.getX();
	pos.y = hQuadro * tuioCursor.getY();
	
	vector<Finger*>::iterator it;
	for (it=dito_vec.begin(); it !=dito_vec.end(); ++it) 
	{
		if ( (*it)->get_finger_id() == finger_id) 
		{
			(*it)->removed();
			break;
		}
	}
	dito_vec.erase(it);
}

// PLAY ////////////////////////////////////////////////////////////////
// questo metodo prende in ingresso un numero intero che varia tra 0 - 7
// si occupa di inviare un 'bang' a PureData (inviandolo al receiver denominato "colon_bang")
// si occupa inoltre di analizzare la matrice dei suoni per capire quali debbano essere messi in riproduzione
//
void ofApp::play(int colonna) 
{
	
	int piano;
	int riga;
	//char *s;
	std::string s;

	ofxOscMessage m;
	m.setAddress("/transport/column_num");
	m.addIntArg(colonna);
	sender.sendMessage(m);
	
	for (piano=1; piano<PLANES; ++piano)
	{
		for(riga=0; riga<ROWS; ++riga)
		{
			if (matrice.get_element(riga, colonna, piano)==1) 
			{
				switch (piano)
				{
					case 1:
						s="kick";
						break;
					case 2:
						s="snare";
						break;
					case 3:
						s="hihat";
						break;
					default:
						cout << "Unavailable Instrument." << endl;
						break;
				}

				std::string stringa = "/drums/"+s;
				//cout << stringa << endl;
				//ofxOscMessage m;
				m.clear();
				m.setAddress(stringa);
				m.addStringArg("bang");
				sender.sendMessage(m);
				
				break;
				// questo break è per passare immediatamente al piano successivo
				// non appena si trovi, anche soltanto uno, strumento
				// al piano attuale
			}
		}
	}
}
		
//----------------------------------------------------------
// questa è una versione modificata rispetto a quella presente di default nel parco di funzioni 
// fornite da OpenFrameworks versione 0.74. La funzione originale è la ofBackgroundGradient() .
void ofApp::backgroundGradient(const ofColor& start, const ofColor& end, int w_, int h_) 
{
	float w = w_, h = h_;
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
	
		// this could be optimized by building a single mesh once, then copying
		// it and just adding the colors whenever the function is called.
		ofVec2f center(w / 2, h / 2);
		mesh.addVertex(center);
		mesh.addColor(start);
		int n = 32; // circular gradient resolution
		float angleBisector = TWO_PI / (n * 2);
		float smallRadius = ofDist(0, 0, w / 2, h / 2);
		float bigRadius = smallRadius / cos(angleBisector);
		for(int i = 0; i <= n; i++) {
			float theta = i * TWO_PI / n;
			mesh.addVertex(center + ofVec2f(sin(theta), cos(theta)) * bigRadius);
			mesh.addColor(end);
		}
	
	glDepthMask(false);
	mesh.draw();
	glDepthMask(true);
}

//----------------------------------------------------------
void ofApp::meshReset() 
{
	//ofLog() << "\tMESH RESET: inizializzazione della mesh" << endl;
	cout << "\tMESH RESET: inizializzazione della mesh" << endl;
	// nella fase di setup iniziale del tavolo può essere necessario 
	// reimpostare i vari valori a quelli iniziali.
	
	meshRotationX	= 6.0f;		// valore in gradi 
    meshRotationY   = 0.0f;
    meshRotationZ   = 1.0f;
	meshCenterX		= 10.0f;
    meshCenterY     = -86.0f;
    meshCenterZ     = -146.5f;
}

/*
//----------------------------------------------------------
void ofApp::rvReset() 
{
	//ofLog() << "\tRV RESET" << endl;
	cout << "\tRV RESET: reset delle trasformazioni per le coordinate reacTIVision" << endl;

	rvTranslateX	= 0.0f;
	rvTranslateY	= 0.0f;
	rvScaleX		= 1.0f;
	rvScaleY		= 1.0f;
	//rvRotateZ		= 0.0f;		// valore in gradi 
}
*/


//----------------------------------------------------------
void ofApp::fboLogosFilling(int w_, int h_) 
{
	int w = w_;
	int h = h_;
	
	int imgA_scale = 4.5;
	int imgB_scale = 3;
	
	float d = 15;
	
	fboLogos.allocate(w, h, GL_RGB);
		
	// immagini normali
	imgA.loadImage("images/logoTipo_white.png");
	//imgA.loadImage("images/logoTipo_transparency.png");
	imgB.loadImage("images/Limulo_quadro_LOW_RES_fondo_bianco.tif");
	
	imgA.resize(imgA.getWidth()/imgA_scale, imgA.getHeight()/imgA_scale);
	imgB.resize(imgB.getWidth()/imgB_scale, imgB.getHeight()/imgB_scale);
	
	imgA.setAnchorPercent(0.88, 0.5);
	imgB.setAnchorPercent(0.5, 0.5);
	
	// immagini girate
	imgA_180.clone(imgA);
	imgB_180.clone(imgB);
	
	imgA_180.rotate90(2);
	imgB_180.rotate90(2);
	
	imgA_180.setAnchorPercent(0.125, 0.5);
	imgB_180.setAnchorPercent(0.5, 0.5);
	
	float margine = (40 -d)/80;
	
	fboLogos.begin();
		ofClear(255,255,255, 0);
		
		ofPushStyle();
		ofPushMatrix();
		ofTranslate(w * 0.5, hQuadro * 0.5);
			ofPushMatrix();
			ofTranslate(w * margine * -1, h * margine);
				imgB.draw( 0, 0 );
			ofPopMatrix();
	
			ofPushMatrix();
			ofTranslate(w * margine, h * margine);
				imgA.draw( 0, 0 );
			ofPopMatrix();
	
	
			ofPushMatrix();
			ofTranslate(w * margine, h * margine * -1);
				imgB_180.draw( 0, 0 );	
			ofPopMatrix();
		
			ofPushMatrix();
			ofTranslate(w * margine * -1, h * margine * -1);
				imgA_180.draw( 0, 0 );
			ofPopMatrix();
		ofPopMatrix();
		ofPopStyle();
	fboLogos.end();
	
	ofPixels p;
	fboLogos.readToPixels(p, 0);
	
	newLogosImage.setFromPixels(p);
	
	// se ci fosse necessità di salvare l'immagine, togliere il commento dalla riga seguente
	//newImage.saveImage("fondo_loghi.png", OF_IMAGE_QUALITY_BEST);
}