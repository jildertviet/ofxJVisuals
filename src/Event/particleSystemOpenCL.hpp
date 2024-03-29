//
//  particleSystemOpenCL.hpp
//  Visualizer_1_2
//
//  Created by Jildert Viet on 04-12-16.
//
//
#if USE_OPENCL

#ifndef particleSystem_hpp
#define particleSystem_hpp

#include <stdio.h>
#include "ofMain.h"
#include "JEvent.hpp"
#include "MSAOpenCL.h"
#include "JVecField.hpp"


class particleSystem: public JEvent{
public:
    particleSystem();
    particleSystem(int numParticles, ofVec2f size = ofVec2f(1280, 800), ofFloatColor color=ofFloatColor(1.0, 1.0, 1.0, 0.05), int testIndex=1, string clFile = "MSAOpenCL/Particle.cl");
    void init(int numParticles, string clFile = "MSAOpenCL/Particle.cl");
    unsigned int numParticles = 1000000;
    virtual void specificFunction() override;
    void display() override;
    ofFloatColor color = ofFloatColor(1.0, 1.0, 1.0, 0.05);

    float2* particlesPos;
    float2 dimensions;

    msa::OpenCL	opencl;
    GLuint vbo, cbo;
    bool mouseControl = false;

    msa::OpenCLImage clImage;
    float2 globalForce = float2(0, 0);
    float2 forceMultiplier = float2(1, 1);
    bool bMirrorMouse = false;
    void setVecField(JVecField* vF);
    void customOne(float* v = nullptr) override;
    void customTwo(float* v = nullptr) override;
    void customThree(float* v = nullptr) override;
    void customFour(float* v = nullptr) override;
    void customFive(float* v = nullptr) override;
    void setSize(glm::vec3 size) override;
    float traagheid = 0.9;
    float fadeTime = 0.001;
    float destAlpha = 0.4;
    void setColor(ofColor c, int index=0) override;
    virtual bool copyVecFieldToCLImage();
    JVecField* vecField = nullptr;
    unsigned char* pixels;
private:
};


#endif /* particleSystem_hpp */
#endif // USE_OPENCL
