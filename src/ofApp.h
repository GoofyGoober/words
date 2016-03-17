#pragma once

#include "ofMain.h"
#include "WordsHandler.h"
#include "ofxLibwebsockets.h"
#include "ofxBlur.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{ 

public:
  void                      setup();
  void                      update();
  void                      draw();
  void                      resetWords();
  void                      keyPressed(int key);
  void                      setupWebSocket();
  void                      setupGui();
  void                      onConnect( ofxLibwebsockets::Event& args );
  void                      onOpen( ofxLibwebsockets::Event& args );
  void                      onClose( ofxLibwebsockets::Event& args );
  void                      onIdle( ofxLibwebsockets::Event& args );
  void                      onMessage( ofxLibwebsockets::Event& args );
  void                      onBroadcast( ofxLibwebsockets::Event& args );
  void                      audioIn(float * input, int bufferSize, int nChannels);
  void                      setupAudio();
  void                      updateAudio();
  
  WordsHandler              words;
  ofxLibwebsockets::Server  server;
  bool                      bSetup;
  vector<string>            messages;
  string                    toSend;
  vector<float>             left;
  vector<float>             right;
	vector<float>             volHistory;
  int                       bufferCounter;
  int                       drawCounter;
  float                     smoothedVol;
  float                     scaledVol;
  ofSoundStream             soundStream;
  vector<string>            foundWords;
  ofImage                   overlay;
  ofxBlur                   blur;
  ofParameter<bool>         duplicateWords;
  ofParameter<float>        blurMultiplier;
  ofParameter<bool>         screamForClean;
  ofParameter<float>        scaledVolumeLimitScremForClean;
  ofParameter<float>        speedDisappear;
  ofParameter<float>        limitFramerate;
  ofxPanel                  gui;
  bool                      showGUI;
  ofParameter<float>        blurValue;
  ofVec2f                   windowsSize;
};
