// //
// //  meshWalker.hpp
// //  meshTrails
// //
// //  Created by Jildert Viet on 18-01-18.
// //
// //
//
// #ifndef meshWalker_hpp
// #define meshWalker_hpp
//
// #include <stdio.h>
// #include "ofMain.h"
// #include "JEvent.hpp"
//
// class meshWalker: public JEvent{
// public:
// typedef unsigned int uint32;
// typedef unsigned short uint16;
//
//     meshWalker(){};
//     meshWalker(ofMesh* m, glm::vec2 dimensions);
//     void specificFunction();
//     void display();
//     ofMesh* m;
//     glm::vec3 loc = glm::vec3(0,0,0);
//     glm::vec3 locOnGrid = glm::vec3(0,0,0);
//     glm::vec3 destination = glm::vec3(1, -1, -1);
//     bool getDestination(glm::vec2 d);
//     uint32 numPoints;
//     glm::vec2 dimensions;
//     uint32 meshIndex = 0;
//     glm::vec3 direction = glm::vec3(0,1,0);
//     float speed = 5; // time to get to next point
//     float travelDistance;
//     uint32 numSteps;
//     uint32 numStepsStart;
//     glm::vec3 getPerlinDirection();
//
//     glm::vec3 getDirection();
//     glm::vec3 globalDirection = glm::vec3(1,1);
//
//     float perlinReadPoint = 0;
//
//     vector<glm::vec3> trailPoints;
//     uint16 trailLength = 80;
//     uint16 fadeLength = 40;
//
//     ofMesh trail;
//     ofFloatColor color = ofFloatColor(1.0, 1.0, 1.0, 1.0);
//
//     bool bDrawCircles = false;
//     float trainLengthRatio = 0.5;
// //    float segmentLength;
// //    float diagonalSegmentLength;
//     void displayTrail();
//     float lineWidth = 3;
//     void addFade();
//     bool staticDirection = true;
// private:
//     void addFadeFromStart();
// };
// #endif /* meshWalker_hpp */
