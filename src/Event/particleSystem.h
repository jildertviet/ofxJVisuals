#if USE_OPENCL == false

#ifndef particleSystem_hpp
#define particleSystem_hpp

#include <stdio.h>
#include "ofMain.h"
#include "JEvent.hpp"
#include "JVecField.hpp"
#include "JPPBuffer.h"

class particleSystem: public JEvent{
public:
    particleSystem();
    particleSystem(
      int numParticles,
      glm::vec2 size = glm::vec2(1280, 800)
    );
    void init(int numParticles);
    void specificFunction() override;
    void display() override;
    void setVecField(JVecField* vF);
    JVecField* vecField = nullptr;
    
    	ofShader    updatePos;
    	ofShader    updateVel;
    	ofShader    updateRender;

    	pingPongBuffer posPingPong;
    	pingPongBuffer velPingPong;

    	ofFbo   renderFBO;

    	ofImage sparkImg;

    	float   timeStep;
    	float   particleSize;

    	int     width, height;
    	int     imgWidth, imgHeight;
    	int     numParticles;
    	int     textureRes;


    	ofVboMesh mesh;
private:
};

#endif /* particleSystem_hpp */
#endif // USE_OPENCL
