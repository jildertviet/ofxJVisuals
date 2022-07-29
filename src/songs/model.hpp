//
//  model.hpp
//  Visualizer_1_5
//
//  Created by Jildert Viet on 20-01-17.
//
//

#ifndef model_hpp
#define model_hpp

#include <stdio.h>
#include "Song.hpp"
#include "ofxAssimpModelLoader.h"
#include "meshWalker.hpp"
#include "cameraController.hpp"

class jMesh: public JEvent{
public:
    jMesh(){};
    void specificFunction(){
        rotation += rotationSpeed;
    }
    void display(){
        ofPushMatrix();
        ofTranslate(offset);
        if(rotationSpeed.x != 0)
            ofRotateX(rotation.x);
        if(rotationSpeed.y != 0)
            ofRotateY(rotation.y);
        if(rotationSpeed.z != 0)
            ofRotateZ(rotation.z);

        ofSetColor(colors[0]);
        m.drawWireframe();
        m2.drawWireframe();
        ofPopMatrix();
    }

    ofVec3f offset;
    ofMesh m;
    ofMesh m2;
    ofVec3f rotationSpeed = ofVec3f(0, 0, 0);
    ofVec3f rotation = ofVec3f(0, 0, 0);
};

class model: public Song{
public:
    model(ofxJVisuals* v);
    
    void F0(); void F1(); void F2(); void F3();
    void F4(); void F5(); void F6(); void F7();
    void F8(); void F9(); void F10(); void F11();
    void F12(); void F13(); void F14(); void F15();
    
    void C0(int); void C1(int); void C2(int); void C3(int);
    void C4(int); void C5(int); void C6(int);
    
    void exit();
    
    ofxAssimpModelLoader m;
    vector<jMesh*> meshes;
    int attack = 100;
    int release = 200;
    float xPosAttackScale = 1;
    float xPosReleaseScale = 1;
    cameraController* camController;

};

#endif /* model_hpp */
