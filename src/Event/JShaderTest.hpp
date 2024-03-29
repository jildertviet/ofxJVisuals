//
//  JShaderTest.hpp
//  Visualizer_2_8
//
//  Created by Jildert Viet on 14-10-19.
//

#ifndef JShaderTest_hpp
#define JShaderTest_hpp

#include <stdio.h>
#include "JEvent.hpp"
#include "ofMain.h"

class JShaderTest: public JEvent{
public:
    JShaderTest(glm::vec2 size = glm::vec2(1280, 800));
    void specificFunction() override;
    void display() override;
    ofShader shader;
    ofFbo mask;
    ofFbo src;
    int noiseSeed;
    ofVec2f divisions = ofVec2f(16, 16);
    void customOne(float* v = nullptr) override;
};
#endif /* JShaderTest_hpp */
