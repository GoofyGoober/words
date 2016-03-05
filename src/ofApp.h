#pragma once

#include "ofMain.h"
#include "WordsHandler.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    void keyPressed(int key);
    WordsHandler  words;
};
