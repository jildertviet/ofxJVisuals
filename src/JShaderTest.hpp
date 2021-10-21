//
//  JShaderTest.hpp
//  Visualizer_2_8
//
//  Created by Jildert Viet on 14-10-19.
//

#ifndef JShaderTest_hpp
#define JShaderTest_hpp

#include <stdio.h>
#include "Event.hpp"
#include "ofMain.h"

class JShaderTest: public Event{
public:
    JShaderTest(ofVec2f size = ofVec2f(1280, 800));
    void specificFunction() override;
    void display() override;
    ofShader shader;
    ofFbo mask;
    ofFbo src;
    int noiseSeed;
    ofVec2f divisions = ofVec2f(16, 16);
    void customOne() override;
};
#endif /* JShaderTest_hpp */
