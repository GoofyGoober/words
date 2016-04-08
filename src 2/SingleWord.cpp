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

void  SingleWord::setup(ofVec2f startPoint, string word, float life)
{
  this->word = word;
//  this->font = font;
  color = ofColor(ofRandom(100,240));
  this->life = life;
  disappear = 255;
  disapperSpeed = 10;
}

void  SingleWord::draw()
{
  float alpha = 255;
  life-=10;
  if(((ofApp*)ofGetAppPtr())->words.singleWords.size() > 80 && ofGetFrameRate() < 15)
  {
    life -= 20;
    disapperSpeed = 50;
  }
  if(life <= 0)
  {
    life = 0;
    disappear -= disapperSpeed;
    alpha = disappear;
  }
  if(alpha > 0)
  {
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(rect.x, rect.y);
    //  drawDebug();
    ofSetColor(color,alpha);
    ofPushMatrix();
    ofTranslate(0, rect.height);
    ofTranslate(-rect.width/100*1, -rect.height/100*1);
    ofScale(scale, scale);
    
    ((ofApp*)ofGetAppPtr())->words.font.drawStringAsShapes(word, 0, 0);
    ofPopMatrix();
    ofPopMatrix();
    ofPopStyle();
  }
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