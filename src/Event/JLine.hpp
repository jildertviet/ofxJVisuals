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

typedef unsigned char uint8;

class JLine: public JEvent{
public:
    JLine();
    void display() override;
    void specificFunction() override;
    ofPolyline line;
    int amplitude = 0;
    uint8 numPoints = 10;
    float seed;

    float balance = 0.5;
    int heightOffset = 0;

    float rotation = 0;

    void toRect(ofVec2f loc, ofVec2f size);

    void fromBuffer(int bufferMode = 0);
    void setLoc(glm::vec3 loc) override;
    void setSize(glm::vec3 size) override;
    void calcLine();

    void customOne(float* v = nullptr) override;
    void customTwo(float* v = nullptr) override;
};
#endif /* JLine_hpp */
