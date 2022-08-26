//
//  JLine.hpp
//  Visualizer_2_3
//
//  Created by Jildert Viet on 18-09-17.
//
//

#ifndef JLine_hpp
#define JLine_hpp

#include <stdio.h>
#include "ofMain.h"
#include "JEvent.hpp"

class JLine: public JEvent{
public:
typedef unsigned char uint8;
    JLine();
    void display();
    void specificFunction();
    ofPolyline line;
    int amplitude = 0;
    uint8 numPoints = 10;
    float seed;
    
    float balance = 0.5;
    int heightOffset = 0;
    
    float rotation = 0;
    
    void toRect(ofVec2f loc, ofVec2f size);
    
    uint8 mode = 0;
};
#endif /* JLine_hpp */
