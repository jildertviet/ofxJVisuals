//
//  dotsBlob.hpp
//  dots
//
//  Created by Jildert Viet on 10-04-18.
//

#ifndef dotsBlob_hpp
#define dotsBlob_hpp

#include <stdio.h>
#include "ofMain.h"
#include "Event.hpp"

class dotsConnectionLine{
public:
    dotsConnectionLine(){};
    void update(){};
    void display(){};
};

struct dotsCircle{
    float rotation = ofRandom(360.);
    float r = ofRandom(10, 50);
    float speed = ofRandom(0.1, 1.0);
    ofVec3f axis = ofVec3f(1, 0, 0);
};

class dotsBlob: public Event{
public:
typedef unsigned short uint16;
    float speed = 0;
    ofVec3f dir;
    
    bool bOnlyLines = false;
    bool bDrawSphere = false;
    dotsBlob();
    void display();
    void update();
    ofVec3f loc = ofVec3f(0, 0, 0);
    vector<dotsBlob*> getBlobsInRange();
    vector<dotsBlob*>* blobs;
    vector<dotsBlob*> connections;
    vector<dotsCircle> circles;
    void updateCircles();
    void drawCircles(ofColor c = ofColor(255, 30));
    
    char numConnections = 0;
    char maxConnections = 8;
    void drawConnections(ofColor c = ofColor(255, 30));
    void calculateConnections();
//    dotsConnectionLine* connections = nullptr;
    
    ofPolyline* connectionL = nullptr;
    ofPolyline* connectionR = nullptr;
    
    float range = 50;
    float startPointRotation = 0;
    float endPointRotation = 0;
    
    bool bDisplayRange = true;
    float percent = 1.0;
    
    float rotationLeft = 0;
    float rotationRight = 0;
    float seed;
    float minDistance = 50;
    
    vector<float> distances;
//    int lineDensity = 50;
    int segmentLength = 2;
    
    void calcPolyBlob();
    ofPolyline polyBlob;
    void drawPolyBlob();
};

#endif /* dotsBlob_hpp */
