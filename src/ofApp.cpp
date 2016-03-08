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
  setupWebSocket();
  resetWords();
  overlay.load("overlayWords.png");
  blur.setup(1920, 1080, 10, .2, 4);
  duplicateWords = true;
}

void ofApp::resetWords()
{
  foundWords.clear();
}

void ofApp::setupWebSocket()
{
  ofxLibwebsockets::ServerOptions options = ofxLibwebsockets::defaultServerOptions();
  options.port = 9092;
  options.bUseSSL = false; // you'll have to 	manually accept this self-signed cert if 'true'!
  bSetup = server.setup( options );
  // this adds your app as a listener for the server
  server.addListener(this);
}

//--------------------------------------------------------------
void ofApp::update(){
  blur.setScale(ofMap(mouseX, 0, ofGetWidth(), 0, 10));
  blur.setRotation(ofMap(mouseY, 0, ofGetHeight(), -PI, PI));
}

//--------------------------------------------------------------
void ofApp::draw(){
  ofBackground(0);
  blur.begin();
  ofSetColor(255);
  words.draw();
  blur.end();
  
  blur.draw();

  ofPushStyle();
  overlay.draw(0,0);
  ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  if(key == 'n')
    words.addNewWord();
}

void ofApp::onConnect( ofxLibwebsockets::Event& args ){
  cout<<"on connected"<<endl;
}

//--------------------------------------------------------------
void ofApp::onOpen( ofxLibwebsockets::Event& args ){
  cout<<"new connection open"<<endl;
  messages.push_back("New connection from " + args.conn.getClientIP() + ", " + args.conn.getClientName() );
}

//--------------------------------------------------------------
void ofApp::onClose( ofxLibwebsockets::Event& args ){
  cout<<"on close"<<endl;
  messages.push_back("Connection closed");
}

//--------------------------------------------------------------
void ofApp::onIdle( ofxLibwebsockets::Event& args ){
  cout<<"on idle"<<endl;
}

//--------------------------------------------------------------
void ofApp::onMessage( ofxLibwebsockets::Event& args )
{
  cout<<"got message "<<args.message<<endl;
  vector<string> strings = ofSplitString(args.message, " ");
  for(int a = 0; a < strings.size(); a++)
  {
    bool found = false;
    if(!duplicateWords)
    {
      for(int i = 0; i < foundWords.size(); i++)
      {
        if(strings[a] == foundWords[i])
          found = true;
      }
    }
    if(!found)
    {
      foundWords.push_back(strings[a]);
      words.addNewWord(strings[a]);
    }
  }
  // trace out string messages or JSON messages!
  if ( !args.json.isNull() ){
    messages.push_back("New message: " + args.json.toStyledString() + " from " + args.conn.getClientName() );
  } else {
    messages.push_back("New message: " + args.message + " from " + args.conn.getClientName() );
  }
  
  // echo server = send message right back!
  args.conn.send( args.message );
}

//--------------------------------------------------------------
void ofApp::onBroadcast( ofxLibwebsockets::Event& args ){
  cout<<"got broadcast "<<args.message<<endl;
}
