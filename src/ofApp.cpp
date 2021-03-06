#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  ofSetLogLevel(OF_LOG_SILENT);
  wordReady = false;
  guiReady  = false;
  syphonOut.setName("Parole parole parole");
}

void ofApp::setupWords()
{
  wordReady = true;
  windowsSize.x = size->x * ofGetWindowWidth();
  windowsSize.y = size->y * ofGetWindowHeight();
  words.setup(windowsSize.x, windowsSize.y);
  ofSetFrameRate(60);
  ofBackground(0);
  setupWebSocket();
  resetWords();
  overlay.load("overlayWords.png");
  blur.setup(windowsSize.x, windowsSize.y, 10, .2, 4);
  setupAudio();
  system("open 'http://localhost:9092/'");
}

void ofApp::setupAudio()
{
  int bufferSize        = 256;
  bufferCounter         = 0;
  drawCounter           = 0;
  smoothedVol           = 0.0;
  scaledVol             = 0.0;
  soundStream.printDeviceList();
  left.assign(bufferSize, 0.0);
  right.assign(bufferSize, 0.0);
  volHistory.assign(400, 0.0);
  soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
}

void ofApp::setupGui(bool showSizeAndPos)
{
  showGUI = false;
  gui.clear();
  gui.setup();
  scaledVolumeLimitScremForClean = 1;
  duplicateWords = false;
  blurMultiplier = 10;
//  gui.add(duplicateWords.set("Duplicate words", false));
//  gui.add(blurMultiplier.set("Blur multiplier", 10, 1, 20));
  gui.add(screamForClean.set("Scream For Clean", true));
//  gui.add(scaledVolumeLimitScremForClean .set("Scaled Volume Limit", 1, 0, 3));
  gui.add(speedDisappear.set("Speed Blur Disappear", 0.0001, 0.0000, .0500));
  gui.add(limitFramerate .set("Limit framerate", 15, 10, 60));
  if(showSizeAndPos)
  {
    gui.add(size.set("size", ofVec2f(.5), ofVec2f(0), ofVec2f(1)));
    gui.add(pos.set("pos", ofVec2f(0), ofVec2f(0), ofVec2f(2000,2000)));
  }
  gui.add(*words.getParameterGroup());
  gui.setPosition(ofPoint(0,ofGetWindowHeight()-400));
  gui.loadFromFile("settings.xml");
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
  blur.setRotation(PI);
  updateAudio();
  if(ofGetFrameRate() < limitFramerate)
    words.frameRateTooLow();

  if(!screamForClean)
    blurValue = scaledVol * blurMultiplier;
  else
  {
    if(scaledVol < scaledVolumeLimitScremForClean)
    {
      blurValue += speedDisappear;;
    }
    else
    {
      blurValue -= .1;
    }
  }
  blurValue = ofClamp(blurValue, 0, 5);
  blur.setScale(blurValue);
}

void ofApp::updateAudio()
{
  //lets scale the vol up to a 0-1 range
  scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
  //lets record the volume into an array
  volHistory.push_back( scaledVol );
  //if we are bigger the the size we want to record - lets drop the oldest value
  if( volHistory.size() >= 400 )
    volHistory.erase(volHistory.begin(), volHistory.begin()+1);
}

//--------------------------------------------------------------
void ofApp::draw(){
  ofBackground(0);  
  if(wordReady)
  {
    
    blur.begin();
    ofSetColor(255);
    words.draw();
    blur.end();
    ofPushMatrix();
    ofTranslate(pos->x, pos->y);
    blur.draw();
    
    ofPushStyle();
    ofSetColor(255,255);
//    cout << windowsSize.x << " AND = " << windowsSize.y << endl;
    overlay.draw(0,0, windowsSize.x, windowsSize.y);
    ofPopStyle();
    ofPopMatrix();
    syphonOut.publishScreen();
  }
  else
  {
    ofPushStyle();
    ofSetColor(255,255,0,100);
    ofDrawRectangle(pos->x, pos->y, size->x * ofGetWindowWidth(), size->y * ofGetWindowHeight());
    ofPopStyle();
  }
  if(showGUI)
  {
    ofDrawBitmapString(ofToString(ofGetFrameRate()), ofGetWindowWidth() - 60, 20);
    ofDrawBitmapString(ofToString(words.singleWords.size()), ofGetWindowWidth() - 40, 40);
    gui.draw();
  }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  if(key == 'n')
    words.addNewWord();
  if(key == 'g')
    showGUI = !showGUI;
  if(key == 'f' && !guiReady)
  {
    guiReady = true;
    ofToggleFullscreen();
    setupGui();
  }
  if(key == 's'&&!wordReady)
  {
    setupWords();
    setupGui(false);
  }
}

void ofApp::onConnect( ofxLibwebsockets::Event& args ){
  ofLog()<<"on connected"<<endl;
}

//--------------------------------------------------------------
void ofApp::onOpen( ofxLibwebsockets::Event& args ){
  ofLog()<<"new connection open"<<endl;
  messages.push_back("New connection from " + args.conn.getClientIP() + ", " + args.conn.getClientName() );
}

//--------------------------------------------------------------
void ofApp::onClose( ofxLibwebsockets::Event& args ){
  ofLog()<<"on close"<<endl;
  messages.push_back("Connection closed");
}

//--------------------------------------------------------------
void ofApp::onIdle( ofxLibwebsockets::Event& args ){
  ofLog()<<"on idle"<<endl;
}

//--------------------------------------------------------------
void ofApp::onMessage( ofxLibwebsockets::Event& args )
{
  ofLog()<<"got message "<<args.message<<endl;
  vector<string> strings = ofSplitString(args.message, " ");
  for(int a = 0; a < strings.size(); a++)
  {
//    if(a > 100)
//      return;
    bool found = false;
    if(!duplicateWords)
    {
      for(int i = 0; i < foundWords.size(); i++)
      {
        if(ofToUpper(strings[a]) == ofToUpper(foundWords[i]))
          found = true;
      }
    }
    if(!found)
    {
      foundWords.push_back(ofToUpper(strings[a]));
      for(int i = 0; i < 5; i++)
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
void ofApp::audioIn(float * input, int bufferSize, int nChannels)
{
  float curVol = 0.0;
  // samples are "interleaved"
  int numCounted = 0;
  //lets go through each sample and calculate the root mean square which is a rough way to calculate volume
  for (int i = 0; i < bufferSize; i++){
    left[i]		= input[i*2]*0.5;
    right[i]	= input[i*2+1]*0.5;
    curVol += left[i] * left[i];
    curVol += right[i] * right[i];
    numCounted+=2;
  }
  //this is how we get the mean of rms :)
  curVol /= (float)numCounted;
  // this is how we get the root of rms :)
  curVol = sqrt( curVol );
  smoothedVol *= 0.93;
  smoothedVol += 0.07 * curVol;
  bufferCounter++;
}


//--------------------------------------------------------------
void ofApp::onBroadcast( ofxLibwebsockets::Event& args ){
  ofLog()<<"got broadcast "<<args.message<<endl;
}
