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
#include "JEvent.hpp"
#include "pathWalker.hpp"
// #include "glm/gtx/transform.hpp"

class meshRectangle: public JEvent{
public:
typedef unsigned short uint16;
typedef unsigned char uint8;
    meshRectangle(glm::vec2 size = glm::vec2(500), glm::vec3 center = glm::vec3(0), bool bWithLines=false);
    void specificFunction();
    void display();
    ofFloatColor color;
    float rotateSpeed = 0;
    float rotateAngle = 0;
    ofMesh m;
    ofMesh connectedLines;
    meshRectangle* connection = nullptr;
    void drawConnection();
    glm::vec3 a, b, c, d;
    vector<pathWalker*> pathWalkers;
    void setPathWakersSpeed(float speed);
    ofFloatColor fillColor = ofFloatColor(255,255,255,50);
    float fillRatio = 0;
    char orientation = 0;
    int bWalk = 0;
    float lineWidth = 5;
};
#endif /* meshRectangle_hpp */
