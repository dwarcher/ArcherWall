#include "wallApp.h"

//--------------------------------------------------------------
void wallApp::setup(){
	testImg.loadImage("wall.jpg");
	testTexture.loadImage("testpattern.png");
	ofSetFullscreen(true);
	ofSetEscapeQuitsApp(true);

	for(int i=0; i<NUMSTRIPS; i++)
	{
		stripFBO[i].allocate(1024, 128, true);

		strips[i].setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

		//mesh.addVertex(ofVec3f(leftPoint.x, leftPoint.y, leftPoint.z));
		//mesh.addVertex(ofVec3f(rightPoint.x, rightPoint.y, rightPoint.z));
	}

	cursorX = 0;
	cursorY = 0;


	currentStrip = 0;
	expectedDivisions = 5;

	myfont.loadFont("Prototype.ttf", 32);

	drawBack = true;
	ph = 0;
	ofSetFrameRate(60);
	/*
	int bufferSize		= 512;
	sampleRate 			= 44100;
	phase 				= 0;
	phaseAdder 			= 440.0 / 44100.0f;
	phaseAdderTarget 	= 0.0f;
	volume				= 1.0f;
	bNoise 				= false;
	pan = 0.5;

	lAudio.assign(bufferSize, 0.0);
	rAudio.assign(bufferSize, 0.0);
	
	//soundStream.listDevices();
	
	//if you want to set the device id to be different than the default
	//soundStream.setDeviceID(1); 	//note some devices are input only and some are output only 

	soundStream.setup(this, 2, 0, sampleRate, bufferSize, 4);


	//soundStream.start();

	*/
	synthSound.loadSound("sounds/synth.wav");
	
}


//--------------------------------------------------------------
void wallApp::update(){

}

//--------------------------------------------------------------
void wallApp::draw(){
	for(int i=0; i<NUMSTRIPS; i++)
	{
		stripFBO[i].begin();

		ofClear(0,0,0,0);
		ofSetColor(255, 0, 0, 255);
		ofRect((ph + (i * 128)) % 1024, 0, 256, 128);
		ph += 1;

		ph %= 1024;

		stripFBO[i].end();
	}

	ofClear(0,0,0,0);

	if(drawBack)
	{
		ofSetColor(255, 255, 255, 255);
		testImg.draw(ofGetWindowRect());

	}
	ofSetColor(255, 0, 0, 255);

	std::stringstream ss;
	ss << "Strip: " << currentStrip;

	myfont.drawString(ss.str().c_str(), 40, 40);

	for(int i=0; i<NUMSTRIPS; i++)
	{
		if(i == currentStrip)
		{
			ofSetColor(255, 128, 128, 255);
		} else {

			ofSetColor(255, 255, 255, 64);

		}
		stripFBO[i].bind();
		strips[i].draw();
		stripFBO[i].unbind();
	}

	ofSetColor(255, 255, 255, 255);

	ofLine(cursorX-10, cursorY, cursorX + 10, cursorY);
	ofLine(cursorX, cursorY-10, cursorX, cursorY+10);

	//testFBO.draw(50, 300);
}

void wallApp::saveAll()
{
	for(int i=0; i<NUMSTRIPS; i++)
	{
		stringstream ss;
		ss << "stream" << i << ".dat";
		strips[i].save(ss.str());
	}

}

void wallApp::loadAll()
{
	for(int i=0; i<NUMSTRIPS; i++)
	{
		stringstream ss;
		ss << "stream" << i << ".dat";
		strips[i].load(ss.str());
	}
}

//--------------------------------------------------------------
void wallApp::keyPressed(int key){
	switch(key)
	{
	case OF_KEY_UP:
		if(currentStrip)
			currentStrip--;
		break;
	case OF_KEY_DOWN:
		if(currentStrip < 9)
			currentStrip++;
		break;
	case OF_KEY_BACKSPACE:
	case OF_KEY_DEL:
		strips[currentStrip].removeVertex(strips[currentStrip].getNumVertices() - 1);
		strips[currentStrip].removeTexCoord(strips[currentStrip].getNumTexCoords() - 1);

		break;

	case 's':
		saveAll();
		break;
	case 'l':
		loadAll();
		break;
	case 'b':
		drawBack = !drawBack;
		break;
	}
}

//--------------------------------------------------------------
void wallApp::keyReleased(int key){

}

//--------------------------------------------------------------
void wallApp::mouseMoved(int x, int y ){
	cursorX = x;
	cursorY = y;

	int width = ofGetWidth();

	float height = (float)ofGetHeight();
	float heightPct = ((height-y) / height);
	targetFrequency = 2000.0f * heightPct;
	phaseAdderTarget = (targetFrequency / (float) sampleRate) * TWO_PI;
}

//--------------------------------------------------------------
void wallApp::mouseDragged(int x, int y, int button){
	cursorX = x;
	cursorY = y;
}

//--------------------------------------------------------------
void wallApp::mousePressed(int x, int y, int button){
	float widthStep = ofGetWidth() / 3.0f;
	if (x < widthStep){
		synthSound.play();
		synthSound.setSpeed( 0.1f + ((float)(ofGetHeight() - y) / (float)ofGetHeight())*10);
		synthSound.setPan(ofMap(x, 0, widthStep, -1, 1, true));
	}
}

//--------------------------------------------------------------
void wallApp::mouseReleased(int x, int y, int button){
	cursorX = x;
	cursorY = y;

	int div = strips[currentStrip].getNumVertices() / 2;
	float u = (float)div / (float)expectedDivisions;
	float v = 0.0;
	if(strips[currentStrip].getNumVertices() & 1)
		v = 1.0;

	strips[currentStrip].addVertex(ofVec3f(x, y, 0));
	strips[currentStrip].addTexCoord(ofVec2f(u * stripFBO[currentStrip].getWidth(), v * stripFBO[currentStrip].getHeight() ));
}

//--------------------------------------------------------------
void wallApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void wallApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void wallApp::dragEvent(ofDragInfo dragInfo) { 

}
/*
void wallApp::audioOut(float * output, int bufferSize, int nChannels){
	//pan = 0.5f;
	float leftScale = 1 - pan;
	float rightScale = pan;

	// sin (n) seems to have trouble when n is very large, so we
	// keep phase in the range of 0-TWO_PI like this:
	while (phase > TWO_PI){
		phase -= TWO_PI;
	}

	if ( bNoise == true){
		// ---------------------- noise --------------
		for (int i = 0; i < bufferSize; i++){
			lAudio[i] = output[i*nChannels    ] = ofRandom(0, 1) * volume * leftScale;
			rAudio[i] = output[i*nChannels + 1] = ofRandom(0, 1) * volume * rightScale;
		}
	} else {
		phaseAdder = 0.95f * phaseAdder + 0.05f * phaseAdderTarget;
		for (int i = 0; i < bufferSize; i++){
			phase += phaseAdder;
			float sample = sin(phase);
			lAudio[i] = output[i*nChannels    ] = sample * volume * leftScale;
			rAudio[i] = output[i*nChannels + 1] = sample * volume * rightScale;
		}
	}

}
*/