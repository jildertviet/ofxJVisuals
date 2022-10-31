#if USE_OPENCL == false

#ifndef particleSystem_hpp
#define particleSystem_hpp

#include <stdio.h>
#include "ofMain.h"
#include "JEvent.hpp"
#include "JVecField.hpp"

class particleSystem: public JEvent{
public:
    particleSystem();
    particleSystem(
      int numParticles,
      glm::vec2 size = glm::vec2(1280, 800),
      ofFloatColor color=ofFloatColor(1.0, 1.0, 1.0, 0.05),
    );
    void init(int numParticles);
    unsigned int numParticles = 1000000;
    virtual void specificFunction() override;
    void display() override;
    ofFloatColor color = ofFloatColor(1.0, 1.0, 1.0, 0.05);

    bool mouseControl = false;

    float2 globalForce = float2(0, 0);
    float2 forceMultiplier = float2(1, 1);
    bool bMirrorMouse = false;

    void setVecField(JVecField* vF);

    void customOne() override;
    void customTwo() override;
    void customThree() override;
    void customFour() override;
    void customFive() override;
    void setSize(glm::vec3 size) override;
    float traagheid = 0.9;
    float fadeTime = 0.001;
    float destAlpha = 0.4;
    void setColor(ofColor c, int index=0) override;
    JVecField* vecField = nullptr;
    unsigned char* pixels;

    ofFbo velocityAndOrigin;
    ofFbo massAndAlpha;
    ofShader particleShader;
private:
};

#endif /* particleSystem_hpp */
#endif // USE_OPENCL
