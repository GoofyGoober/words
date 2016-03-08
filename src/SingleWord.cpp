//
//  SingleWord.cpp
//  words
//
//  Created by XXX on 05/03/16.
//
//

#include "SingleWord.h"
#include "ofApp.h"


SingleWord::SingleWord()
{
  
}

void  SingleWord::setup(ofVec2f startPoint, string word)
{
  this->word = word;
//  this->font = font;
  color = ofColor(ofRandom(100,240));
}

void  SingleWord::draw()
{
  ofPushStyle();
  ofPushMatrix();
  ofTranslate(rect.x, rect.y);
//  drawDebug();
  ofSetColor(color,255);
  ofPushMatrix();
  ofTranslate(0, rect.height);
  ofTranslate(-rect.width/100*1, -rect.height/100*1);
  ofScale(scale, scale);
  
  ((ofApp*)ofGetAppPtr())->words.font.drawStringAsShapes(word, 0, 0);
  ofPopMatrix();
  ofPopMatrix();
  ofPopStyle();
}

void SingleWord::drawDebug()
{
  ofFill();
  ofSetColor(200, 100);
  ofDrawRectangle(0,0,rect.width, rect.height);
}

void SingleWord::addVertex(ofVec2f startPoint, ofVec2f endPoint)
{
  rect.x = startPoint.x;
  rect.y = startPoint.y;
  rect.width = endPoint.x - startPoint.x;
  rect.height = endPoint.y - startPoint.y;
}