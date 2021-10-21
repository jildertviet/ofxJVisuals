//
//  MultiMesh.hpp
//  Bas
//
//  Created by Jildert Viet on 23-03-16.
//
//

#ifndef MultiMeshMaybeTomorrow_hpp
#define MultiMeshMaybeTomorrow_hpp

#include <stdio.h>
#include "ofMain.h"
#include "Event.hpp"
#include "Wavetable.h"

class MultiMeshMaybeTomorrow: public Event{
public:
    MultiMeshMaybeTomorrow(int num = 5, ofFloatColor color = ofFloatColor(1,1,1,1));
    //    ~MultiMesh();
    ofMesh* meshes;
    int numMeshes=0;
    
    void display();
    void specificFunction();
    
    ofFloatColor globalColor;
    float* colorAlphas;
    
    void generateRandomMeshes(int num, ofFloatColor color = ofFloatColor(1,1,1,1));
    void generateSymmetricMeshes(int num, ofFloatColor color = ofFloatColor(1,1,1,1), float radius = 400);
    
    void changeMode(ofPrimitiveMode mode);
    Wavetable*** wavetables;
    Env* fadeOutEnv; bool bFadeOut = false;
    Env* fadeInEnv; bool bFadeIn = false;
    
    void morph(); bool bMorph = false; void doMorph(float angle, float time); float morphAngle, morphTime;
    
    void deleteWithFade(int releaseTime, int attackTime=0.0001);
//    void fadeOut();
//    void fadeIn();
    
    void addRandomMesh(float radius, ofFloatColor color = ofFloatColor(1,1,1,1));
    
    void doubleTime(); void halfTime();
    void changeColor(ofFloatColor color);
    void addRadius(float radiusIncrease);
    void growRadius(int direction, float speed); bool bGrowRadius=false;
    ofVec2f** locations;
    
    void centerPulse();
    
    void setFrequency(float freq, float* multipliers, int size);
    
    int bpm = 120;
    
    float radius;
    void changeLocations();
    
    bool bMoveWithPulse = false; void moveWithPulse(); Wavetable* pulseOsc;
    int pulseMoveWidth; int centerPulseRadius = 50;
    
    float radiusAdd = 3;
    bool bCenterPulse = true;
    
    void moveOutVertex(int index); bool bMoveOutVertext = false;
    
    float globalAlphaAdd = 0.1;
    
    void generateJildert();
    
    bool bMoveHorizontal = false; void moveHorizontal();
    bool bMoveVertical = false; void moveVertical();
//    bool checkIfNextVertextHasSameX(ofMesh* vertex, int index);
};
#endif /* MultiMesh_hpp */
