//
//  JVecField.hpp
//  Visualizer_2_5
//
//  Created by Jildert Viet on 09-03-19.
//

#ifndef vecField_hpp
#define vecField_hpp

#include <stdio.h>
#include "Event.hpp"

enum VECFIELD_MODE{
    PERLIN,
    TEST,
    LINES,
    CIRCLES,
    HIDE,
    VIDEO,
    UNDERLAYING,
    TEXTURE
};

class JVecField: public Event{
public:
    JVecField();
    ofShader shader;
//    ofPlanePrimitive plane;
    float shaderContrast = 1.0f;
    float shaderBrightnessAdd = 0.f;;
    ofVideoPlayer* video;
    ofImage frame;
    void specificFunction() override;
    void display() override;
    void setSize(ofVec3f size) override;
    ofVec2f offset;
    ofVec2f sizeMultiplier;
    ofVec2f start, end; // For each line
    ofTexture vecTex;
    ofVec2f getField(ofVec2f pos);
    ofFloatPixels p;
    float lineLength = 10;
    float lineWidth = 1;
    float t;
    float complexity = 20; // wind complexity
    float phase = TWO_PI;
    void setDensity(glm::vec2 d, char numChannels=4);
    bool bDrawColors = false;
    bool bNormalize = true;
    glm::vec2 density = glm::vec2(40, 40);
    char mode = VECFIELD_MODE::UNDERLAYING;
    char drawMode = VECFIELD_MODE::TEXTURE;
    void setMode(char m) override;
    void setPixelsToTest();
    int seed = 0;
    
    ofFbo resizeFbo; // TEST
    ofFbo* underlayer;
    void customOne() override;
private:
    
};
#endif /* JVecField_hpp */
