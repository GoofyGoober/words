#pragma once

#include "ofMain.h"
#include "WordsHandler.h"
#include "ofxLibwebsockets.h"
#include "ofxBlur.h"

class ofApp : public ofBaseApp{ 

	public:
		void          setup();
		void          update();
		void          draw();
    void          resetWords();
    void          keyPressed(int key);
    void          setupWebSocket();
    WordsHandler  words;
  
    ofxLibwebsockets::Server server;
    bool bSetup;
  
    //queue of rec'd messages
    vector<string> messages;
  
    //string to send to clients
    string toSend;
  
    // websocket methods
    void onConnect( ofxLibwebsockets::Event& args );
    void onOpen( ofxLibwebsockets::Event& args );
    void onClose( ofxLibwebsockets::Event& args );
    void onIdle( ofxLibwebsockets::Event& args );
    void onMessage( ofxLibwebsockets::Event& args );
    void onBroadcast( ofxLibwebsockets::Event& args );
  
    vector<string> foundWords;
    ofImage overlay;
    ofxBlur blur;
  ofParameter<bool> duplicateWords;
};
