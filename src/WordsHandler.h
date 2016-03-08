//
//  WordsHandler.h
//  words
//
//  Created by XXX on 05/03/16.
//
//

#ifndef __words__WordsHandler__
#define __words__WordsHandler__

#include "ofMain.h"
#include "SingleWord.h"

class WordsHandler
{
public:
                      WordsHandler();
  void                setup(int width, int height);
  void                draw();
  void                addNewWord(string newWord = "cacca");
   ofTrueTypeFont      font;
  
private:
  void                setupPixels();
  ofVec2f             getFirstPoint();
  ofVec2f             getNewWordSize(ofVec2f startPoint, SingleWord& singleWord);
  ofPixels            pixels;
  vector<SingleWord>  singleWords;
  ofImage             debugImage;
  ofVec2f             windowsSize;
  float               marginFromBorder;
  float               marginFromWords;
  int                 contains(ofRectangle& R1, ofRectangle& R2);
  ofRectangle         checkIfFree(ofRectangle& rect);
  bool valueInRange(int value, int min, int max)
  {
    return (value >= min) && (value <= max);
  }
  ofFbo mainFbo;
};



#endif /* defined(__words__WordsHandler__) */
