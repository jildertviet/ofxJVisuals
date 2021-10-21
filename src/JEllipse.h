//
//  Ellipse.h
//  Batobe_met_MIDI
//
//  Created by Jildert Viet on 07-06-15.
//
//

#ifndef __Batobe_met_MIDI__Ellipse__
#define __Batobe_met_MIDI__Ellipse__

#include <iostream>
#include "ofMain.h"
#include "Wavetable.h"
#include "Event.hpp"

class JEllipse: public Event{
public:
    JEllipse();
    
    float radius;
    float speed;
    ofVec2f size;
    ofVec2f originalSize;
    int alpha;
    int originalAlpha;
    
    float angle;
    void specificFunction();
    
    void display();
    
    Wavetable wavetable;
    bool LFO_alpha = false;
    
    void halfTime();
    bool oneFrameLarger = false;
    bool fill_oneFrame = false;
    
    bool smaller = false;
    int smaller_amount = 10;
    void makeSmaller();
    bool already_haftime = false;
    
};
#endif /* defined(__Batobe_met_MIDI__Ellipse__) */
