//
//  cameraController.hpp
//  versesVideo
//
//  Created by Jildert Viet on 24-01-18.
//
//

#ifndef cameraController_hpp
#define cameraController_hpp

#include <stdio.h>
#include "ofMain.h"
#include "Event.hpp"

class cameraController: public Event{
public:
    cameraController(ofEasyCam* c);
    ofEasyCam* cam;
    void specificFunction();
    float boomSpeed = 0;
    float dollySpeed = 0;
    float truckSpeed = 0;
    ofVec3f orbitSpeeds = ofVec3f(0,0,0);
    ofVec3f orbitValues = ofVec3f(0,0,0);
    ofVec3f rotateSpeed = ofVec3f(0,0,0);
    ofVec3f rotation = ofVec3f(0,0,0);
    ofVec3f rotationSpeed = ofVec3f(0,0,0);
    float orbitDistance = 500;
    
    ofVec3f* focus = nullptr;
};
#endif /* cameraController_hpp */
