//
//  SingleWord.h
//  words
//
//  Created by XXX on 05/03/16.
//
//

#ifndef __words__SingleWord__
#define __words__SingleWord__

#include "ofMain.h"

class SingleWord
{
public:
                        SingleWord();
  void                  setup(ofVec2f startPoint, string word);
  void                  draw();
  void                  addVertex(ofVec2f startPoint, ofVec2f endPoint);
  
  float                 scale;
  string                word;
  ofRectangle           rect;
private:
  void                  drawDebug();
  ofColor               color;
//  ofTrueTypeFont*       font;
  
};

#endif /* defined(__words__SingleWord__) */
