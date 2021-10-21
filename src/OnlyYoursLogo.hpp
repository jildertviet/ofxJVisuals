//
//  OnlyYoursLogo.hpp
//  Visualizer_2_4
//
//  Created by Jildert Viet on 06-10-17.
//
//

#ifndef OnlyYoursLogo_hpp
#define OnlyYoursLogo_hpp

#include <stdio.h>
#include "ofMain.h"
#include "Event.hpp"

class OnlyYoursLogo: public Event{
    public:
    OnlyYoursLogo();
    void display();
    
    char mode = 0; // 0 is angle, 1 is rect
    
    void displayRects(ofColor c = ofColor::white);
    void drawLinesLeft();
    void drawLinesRight();
    void displayOneLayer(ofColor c = ofColor::white);
    
    
    int lineWidth = 2;
    ofVec2f layerOffset = ofVec2f(100,20);
    float scale;
    
    short int numLayers = 3;
    
    void setAlpha(char alpha, int depth=4);
    
    vector<ofVec2f> bezierPoints;
};
#endif /* OnlyYoursLogo_hpp */
