//
//  meshRectangle.hpp
//  versesVideo
//
//  Created by Jildert Viet on 27-01-18.
//
//

#ifndef meshRectangle_hpp
#define meshRectangle_hpp

#include <stdio.h>
#include "Event.hpp"
#include "pathWalker.hpp"
#include "glm/gtx/transform.hpp"
class meshRectangle: public Event{
public:
typedef unsigned short uint16;
typedef unsigned char uint8;
    meshRectangle(ofVec2f size = ofVec2f(500,500), ofVec3f center = ofVec3f(0,0,0), bool bWithLines=false);
    void specificFunction();
    void display();
    ofFloatColor color;
    float rotateSpeed = 0;
    float rotateAngle = 0;
    ofMesh m;
    ofMesh connectedLines;
    meshRectangle* connection = nullptr;
    void drawConnection();
    ofVec3f a, b, c, d;
    vector<pathWalker*> pathWalkers;
    void setPathWakersSpeed(float speed);
    ofFloatColor fillColor = ofFloatColor(255,255,255,50);
    float fillRatio = 0;
    char orientation = 0;
    int bWalk = 0;
    float lineWidth = 5;
};
#endif /* meshRectangle_hpp */
