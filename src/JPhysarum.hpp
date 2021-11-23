//
//  JPhysarum.hpp
//  visualizer
//
//  Created by Jildert Viet on 01-07-20.
//

#ifndef JPhysarum_hpp
#define JPhysarum_hpp

#define PHYSARUMGUI false
#include <stdio.h>
#include "ofMain.h"
#include "Event.hpp"
#if PHYSARUMGUI
    #include "ofxGui.h"
#endif

struct pingPongBuffer {
public:
    void allocate( int _width, int _height, int _internalformat = GL_RGBA){
        // Allocate
        for(int i = 0; i < 2; i++){
            FBOs[i].allocate(_width,_height, _internalformat );
            FBOs[i].getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
        }
        
        //Assign
        src = &FBOs[0];
        dst = &FBOs[1];
        
        // Clean
        clear();
    }
    
    void swap(){
        std::swap(src,dst);
//        ofFbo* t = src;
//        src = dst;
//        dst = t;
    }
    
    void clear(){
        for(int i = 0; i < 2; i++){
            FBOs[i].begin();
            ofClear(0,255);
            FBOs[i].end();
        }
    }
    
    ofFbo& operator[]( int n ){ return FBOs[n];}
    ofFbo   *src;       // Source       ->  Ping
    ofFbo   *dst;       // Destination  ->  Pong
    
private:
    ofFbo   FBOs[2];    // Real addresses of ping/pong FBO«s
};

class JPhysarum: public Event{
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
    
private:
    ofShader    updatePos;
    ofShader    updateVel;
    ofShader    updateRender;
    ofShader    alphaDecay;
    
    pingPongBuffer posPingPong;
    pingPongBuffer velPingPong;
    pingPongBuffer renderPingPong;
    
    ofTexture* externalVelocity = nullptr;
    
    int numParticles;
    int textureRes;
    ofVboMesh mesh;
    
    bool bDrawGui = true;
    
#if PHYSARUMGUI
    ofxPanel gui;
#endif

};
#endif /* JPhysarum_hpp */
