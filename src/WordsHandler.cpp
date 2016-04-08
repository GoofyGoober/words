//
//  WordsHandler.cpp
//  words
//
//  Created by XXX on 05/03/16.
//
//

#include "WordsHandler.h"
#include "ofApp.h"


WordsHandler::WordsHandler()
{
  marginFromBorder  = 10;
  marginFromWords   = 1;
  font.load("HelveticaHv.ttf", 200, true, true, true);
  
}

void WordsHandler::setup(int width, int height)
{
  windowsSize.x = width;
  windowsSize.y = height;
  mainFbo.allocate(width, height);
  mainFbo.begin();
  ofClear(0,255);
  mainFbo.end();
  setupPixels();
  timerNoWords = 0;
}


bool checkDead(SingleWord &p){
  return p.disappear <= 0;
}

void WordsHandler::draw()
{
  mainFbo.begin();
  ofClear(0,255);
  int totWords = singleWords.size();
  for(int a = totWords - 1; a >= 0 ; a--)
  {
    singleWords[a].draw();
    if(singleWords[a].disappear <= 0)
    {
      int wordsDuplicatedTot = ((ofApp*)ofGetAppPtr())->foundWords.size();
      for(int i = wordsDuplicatedTot - 1; i >= 0; i--)
      {
        if(singleWords[a].word == ((ofApp*)ofGetAppPtr())->foundWords[i])
        {
          ((ofApp*)ofGetAppPtr())->foundWords.erase(((ofApp*)ofGetAppPtr())->foundWords.begin()+i);
        }
      }
      for(int y = singleWords[a].rect.y; y < singleWords[a].rect.y + singleWords[a].rect.height; y++)
      {
        for(int x = singleWords[a].rect.x; x < singleWords[a].rect.x + singleWords[a].rect.width; x++)
        {
          pixels.setColor(x, y, ofColor(255));
        }
      }
    }
  }
  ofRemove(singleWords, checkDead);
  mainFbo.end();
  mainFbo.draw(0,0);
  
  if(singleWords.size() <= 0)
    timerNoWords++;
  int frameRate = ofGetFrameRate();
  if(frameRate < 10)
    frameRate = 60;
  float minute = frameRate * 60;
  if(timerNoWords >= minute*1 && singleWords.size() == 0)
  {
    timerNoWords = 0;
    for(int a = 0; a < 25; a++)
    {
      addNewWord("raccontami");
      addNewWord("non");
      addNewWord("ti");
      addNewWord("giudico");
      addNewWord("parlami");
      addNewWord("ti");
      addNewWord("prego");
    }
  }
}

void WordsHandler::setupPixels()
{
  pixels.allocate(windowsSize.x, windowsSize.y, OF_PIXELS_GRAY);
  for(int a = 0; a < windowsSize.x * windowsSize.y; a++)
    pixels.setColor(a, ofColor(255));
}

void WordsHandler::addNewWord(string newWord)
{
  float minute = 60*60;
  SingleWord tempWord;
  ofVec2f startPoint = getFirstPoint();
  float life = ofRandom(minute * lifeMinMinute, minute * lifeMaxMinute);
  tempWord.setup(startPoint, ofToUpper(newWord), life);
  ofVec2f size = getNewWordSize(startPoint, tempWord);
  if(size.x != -666)
  {
    ofVec2f endPoint = startPoint;
    endPoint.x += size.x;
    endPoint.y += size.y;
    tempWord.addVertex(startPoint, endPoint);
    ofRectangle newRect = tempWord.rect;
    for(int y = newRect.y; y < newRect.y + newRect.height; y++)
    {
      for(int x = newRect.x; x < newRect.x + newRect.width; x++)
      {
        pixels.setColor(x, y, ofColor(0));
      }
    }
    timerNoWords = 0;
    singleWords.push_back(tempWord);
  }
}

ofVec2f WordsHandler::getNewWordSize(ofVec2f startPoint, SingleWord& singleWord)
{
  int newWwidth = getStartRectSize();
  
  ofRectangle box = font.getStringBoundingBox(singleWord.word, 0, 0);
  float prop = float(box.width/box.height);
  int newHeight = newWwidth/prop;
  ofRectangle tempRect = ofRectangle(startPoint.x, startPoint.y, newWwidth, newHeight);
  ofRectangle newRect = checkIfFree(tempRect);
  if(newRect.width < windowsSize.x / 100 * minWordSize)
    return ofVec2f(-666);
    
  if(newRect.x == -666)
  {
    return ofVec2f(-666);
  }
  else
  {
    newWwidth = newRect.width;
    if(newRect.x + newRect.width > windowsSize.x - marginFromBorder)
      newWwidth = windowsSize.x - marginFromBorder - newRect.x;
    newHeight = newWwidth/prop;
    // Controllare se con l'altezza non esco dallo spazio
    if(newHeight + startPoint.y > windowsSize.y - marginFromBorder)
      newHeight = windowsSize.y - marginFromBorder - startPoint.y;
    newWwidth = newHeight*prop;
    singleWord.scale = float(newWwidth/box.width);
    ofVec2f newRectSize = ofVec2f(newWwidth, newHeight);
    return newRectSize;
  }
}

ofRectangle WordsHandler::checkIfFree(ofRectangle& rect)
{
    for(int a = 0; a < singleWords.size(); a++)
    {
      int intersection = contains(rect, singleWords[a].rect);
      int count = 0;
      while(intersection > 0)
      {
        if(intersection == 2)
        {
          rect.width-= (rect.width + rect.x) - singleWords[a].rect.x + 10;
          intersection = contains(rect, singleWords[a].rect);
          
          if(rect.x > rect.x + rect.width)
          {
            intersection = false;
            rect.x = rect.y = -666;
          }
        }
        else if(intersection == 3)
        {
          rect.height-= (rect.y + rect.height)  - singleWords[a].rect.y + 10;
          intersection = contains(rect, singleWords[a].rect);
          if(rect.y > rect.y + rect.height)
          {
            intersection = false;
            rect.x = rect.y = -666;
          }
        }
      }
    }
    return rect;
}


int WordsHandler::contains(ofRectangle& R1, ofRectangle& R2)
{
    bool xOverlap = valueInRange(R1.x, R2.x, R2.x + R2.width) || valueInRange(R2.x, R1.x, R1.x + R1.width);
    bool yOverlap = valueInRange(R1.y, R2.y, R2.y + R2.height) || valueInRange(R2.y, R1.y, R1.y + R1.height);
    if(xOverlap && yOverlap)
    {
      if(xOverlap)
        return 2;
      else if(yOverlap)
        return 3;
    }
    else
      return 0;
}

ofVec2f WordsHandler::getFirstPoint()
{
  ofVec2f point;
  bool free = false;
  while(!free)
  {
    point.x = int(ofRandom(marginFromBorder, windowsSize.x - marginFromBorder));
    point.y = int(ofRandom(marginFromBorder, windowsSize.y - marginFromBorder));
    int red = pixels.getColor(point.x, point.y).r;
    if(red == 255)
      free = true;
  }
  return point;
}

int WordsHandler::getStartRectSize()
{
  int newWidth  = ofRandom(windowsSize.x / 100 * minWordSize, windowsSize.x/4);
  if(singleWords.size() > 40)
    newWidth = ofRandom((windowsSize.x / 100 * minWordSize)/2, windowsSize.x/6);
  if(singleWords.size() > 90)
    newWidth = ofRandom((windowsSize.x / 100 * minWordSize)/4, windowsSize.x/8);
  if(singleWords.size() > 120)
    newWidth = ofRandom((windowsSize.x / 100 * minWordSize)/6, windowsSize.x/16);
  if(newWidth > windowsSize.x - marginFromBorder)
    newWidth = windowsSize.x - marginFromBorder;
  return newWidth;
}

bool WordsHandler::underMaxWords()
{
  return (((ofApp*)ofGetAppPtr())->words.singleWords.size() < maxWords);
}

ofParameterGroup* WordsHandler::getParameterGroup()
{
  if(!wordsParameterGroup)
  {
    wordsParameterGroup = new ofParameterGroup();
  }
  if(wordsParameterGroup->getName() == "")
  {
    wordsParameterGroup->setName("Words");
    wordsParameterGroup->add(lifeMinMinute.set("Life min minute", .1, 0, 5));
    wordsParameterGroup->add(lifeMaxMinute.set("Life max minute", 2, 0, 10));
    wordsParameterGroup->add(minWordSize.set("Min Word Size Perc", 50, 0, 100));
    wordsParameterGroup->add(maxWords.set("Max Words", 80, 0, 1000));
  }
  return wordsParameterGroup;
}

void WordsHandler::frameRateTooLow()
{
  if(singleWords.size() > 20)
  {
    singleWords[0].disapperSpeed = 30;
    singleWords[0].life = 0;
  }
}