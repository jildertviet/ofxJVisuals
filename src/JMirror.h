//
//  Mirror.h
//  Batobe_met_MIDI
//
//  Created by Jildert Viet on 27-05-15.
//
//

#ifndef JMirror_h
#define JMirror_h

#include <iostream>
#include "ofMain.h"
#include "Event.hpp"

class JMirror: public Event{
public:
    JMirror();
    JMirror(ofVec2f size_, ofVec2f loc_);
    ~JMirror();

    ofVec2f view;
    
    void display();
    void specificFunction();
//    ofImage image;
    ofTexture texture;
    float ySpeed;
    int angle = 0;
    void moveTriangle();
    bool moveBackwards;
    
    void reSpawn();
    void displayView();
    
    bool bDisplayMirror = true;
    bool draw_view = false;
    bool draw_sides = false;
    
    void moveUp();
    bool move_up;
    float speed;
    void setSize(ofVec3f s);
};
#endif /* defined(JMirror_h) */
