//
//  meshWalker.hpp
//  meshTrails
//
//  Created by Jildert Viet on 18-01-18.
//
//

#ifndef meshWalker_hpp
#define meshWalker_hpp

#include <stdio.h>
#include "ofMain.h"
#include "Event.hpp"

class meshWalker: public Event{
public:
typedef unsigned int uint32;
typedef unsigned short uint16;
    meshWalker(){};
    meshWalker(ofMesh* m, ofVec2f dimensions);
    void specificFunction();
    void display();
    ofMesh* m;
    ofVec3f loc = ofVec3f(0,0,0);
    ofVec3f locOnGrid = ofVec3f(0,0,0);
    ofVec3f destination = ofVec3f(1, -1, -1);
    bool getDestination(ofVec2f d);
    uint32 numPoints;
    ofVec2f dimensions;
    uint32 meshIndex = 0;
    ofVec3f direction = ofVec3f(0,1,0);
    float speed = 5; // time to get to next point
    float travelDistance;
    uint32 numSteps;
    uint32 numStepsStart;
    ofVec3f getPerlinDirection();
    
    ofVec3f getDirection();
    ofVec3f globalDirection = ofVec3f(1,1);
    
    float perlinReadPoint = 0;
    
    vector<ofVec3f> trailPoints;
    uint16 trailLength = 80;
    uint16 fadeLength = 40;
    
    ofMesh trail;
    ofFloatColor color = ofFloatColor(1.0, 1.0, 1.0, 1.0);
    
    bool bDrawCircles = false;
    float trainLengthRatio = 0.5;
//    float segmentLength;
//    float diagonalSegmentLength;
    void displayTrail();
    float lineWidth = 3;
    void addFade();
    bool staticDirection = true;
private:
    void addFadeFromStart();
};
#endif /* meshWalker_hpp */
