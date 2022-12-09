//
//  JPhysarum.hpp
//  visualizer
//
//  Created by Jildert Viet on 01-07-20.
//

#if !defined(JPhysarum_hpp) && !defined(TARGET_RASPBERRY_PI)
#define JPhysarum_hpp

#define PHYSARUMGUI false
#include <stdio.h>
#include "ofMain.h"
#include "JEvent.hpp"
#if PHYSARUMGUI
    #include "ofxGui.h"
#endif
#include "JPPBuffer.h"

class JPhysarum: public JEvent{
public:
    JPhysarum(glm::vec2 loc, glm::vec2 size);
    void display() override;
    void specificFunction() override;

    float sensorAngle = 45;
    float sensorDistance = 20;
    float turnAngle = 45;
    float depositAmount = 60;
    float balance = 0.5;
    float decay = 0.98;
    float blurMix;

    ofTexture* externalVelocity = nullptr;
private:
    ofShader    updatePos;
    ofShader    updateVel;
    ofShader    updateRender;
    ofShader    alphaDecay;

    pingPongBuffer posPingPong;
    pingPongBuffer velPingPong;
    pingPongBuffer renderPingPong;


    int numParticles;
    int textureRes;
    ofVboMesh mesh;

    bool bDrawGui = true;

#if PHYSARUMGUI
    ofxPanel gui;
#endif

};
#endif /* JPhysarum_hpp */
