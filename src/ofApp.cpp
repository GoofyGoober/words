#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  ofVec2f windowsSize;
  windowsSize.x = 1920;
  windowsSize.y = 1080;
  words.setup(windowsSize.x, windowsSize.y);
  ofSetWindowShape(windowsSize.x, windowsSize.y);
  ofSetFrameRate(60);
  ofBackground(0);
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
  words.draw();
}




//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  if(key == 'n')
    words.addNewWord();
}