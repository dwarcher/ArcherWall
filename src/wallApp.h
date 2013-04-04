#pragma once

#include "ofMain.h"
#include "ofFBOTexture.h"

#define NUMSTRIPS 10

class wallApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		//void audioOut(float * input, int bufferSize, int nChannels);

		void saveAll();
		void loadAll();

		//ofSoundStream soundStream;

		float 	pan;
		int		sampleRate;
		bool 	bNoise;
		float 	volume;

		vector <float> lAudio;
		vector <float> rAudio;
		
		//------------------- for the simple sine wave synthesis
		float 	targetFrequency;
		float 	phase;
		float 	phaseAdder;
		float 	phaseAdderTarget;

		
		ofImage testImg;
		ofxFBOTexture stripFBO[NUMSTRIPS];

		ofImage testTexture;

		ofMesh strips[NUMSTRIPS];

		int expectedDivisions;

		int cursorX;
		int cursorY;

		ofPoint pendingPoint;
		bool isPointPending;

		int currentStrip;

		ofTrueTypeFont myfont;

		int ph;

		ofSoundPlayer  synthSound;
		bool drawBack;
};
