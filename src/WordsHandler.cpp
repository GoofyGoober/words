//
//  WordsHandler.cpp
//  words
//
//  Created by XXX on 05/03/16.
//
//

#include "WordsHandler.h"

WordsHandler::WordsHandler()
{
  marginFromBorder = 150;
  marginFromWords = 1;
  font.load("Impact.ttf", 200, true, true, true);
  
}

void  WordsHandler::setup(int width, int height)
{
  windowsSize.x = width;
  windowsSize.y = height;
  setupPixels();
}

void  WordsHandler::draw()
{
  for(int a = 0; a < singleWords.size(); a++)
  {
    singleWords[a].draw();
  }
}

void WordsHandler::setupPixels()
{
  pixels.allocate(windowsSize.x, windowsSize.y, OF_PIXELS_GRAY);
  for(int a = 0; a < windowsSize.x * windowsSize.y; a++)
    pixels.setColor(a, ofColor(255));
}

void WordsHandler::addNewWord()
{
  SingleWord tempWord;
  ofVec2f startPoint = getFirstPoint();
  tempWord.setup(startPoint, ofToUpper("xxx"), &font);
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
    singleWords.push_back(tempWord);
  }
}

ofVec2f WordsHandler::getNewWordSize(ofVec2f startPoint, SingleWord& singleWord)
{
  int newWwidth = ofRandom(windowsSize.x/2);
  if(singleWords.size() > 20)
    newWwidth = ofRandom(windowsSize.x/5);
  if(singleWords.size() > 40)
    newWwidth = ofRandom(windowsSize.x/10);
  if(singleWords.size() > 60)
    newWwidth = ofRandom(windowsSize.x/20);
  if(newWwidth > windowsSize.x - marginFromBorder)
    newWwidth = windowsSize.x - marginFromBorder;
  ofRectangle box = font.getStringBoundingBox(singleWord.word, 0, 0);
  float prop = float(box.width/box.height);
  int newHeight = newWwidth/prop;
  ofRectangle tempRect = ofRectangle(startPoint.x, startPoint.y, newWwidth, newHeight);
  ofRectangle newRect = checkIfFree(tempRect);
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