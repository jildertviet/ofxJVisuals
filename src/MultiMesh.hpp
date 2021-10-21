//
//  MultiMesh.hpp
//  Bas
//
//  Created by Jildert Viet on 23-03-16.
//
//

#ifndef MultiMesh_hpp
#define MultiMesh_hpp

#include <stdio.h>
#include "ofMain.h"
#include "Event.hpp"
#include "Wavetable.h"

class MultiMesh: public Event{
public:
    MultiMesh(int size = 0, ofFloatColor color = ofFloatColor(1,1,1,1));
//    ~MultiMesh();
    ofMesh* meshes = nullptr;
    int numMeshes=0;
    
    void display() override;
    void specificFunction() override;
    
    ofFloatColor globalColor; // to store floatcolor in
    float globalAlpha = 0;
    float* colorAlphas;
    
    void generateRandomMeshes(int num, ofFloatColor color = ofFloatColor(1,1,1,1));
    void generateSymmetricMeshes(int num, ofFloatColor color = ofFloatColor(1,1,1,1), float radius = 400);
    
    void changeMode(ofPrimitiveMode mode);
    void switchMode();
    
    Wavetable*** wavetables;
    bool bFadeOut = false;
//    Envelope* fadeInEnv;
    bool bFadeIn = false;
    
    void morph(); bool bMorph = false; void doMorph(float angle, float time); float morphAngle, morphTime;
    
    void deleteWithFade(int releaseTime); void fadeOut();
    void fadeIn();
    
    void addRandomMesh(float radius, ofFloatColor color = ofFloatColor(1,1,1,1));
    
//    void doubleTime(); void halfTime();
    void changeColor(ofFloatColor color);
    void addRadius(float radiusIncrease);
    void growRadius(int direction, float speed); bool bGrowRadius=false;
    ofVec3f** locations;
    
    void centerPulse();
    
    void setFrequency(float freq, float* multipliers, int size);
    void setFrequency(float freq, float a, float b, float c);
    
    int bpm = 120;
    
    float radius;
    void changeLocations();
    
    void moveWithPulse(); Wavetable* pulseOsc;
    int pulseMoveWidth; int centerPulseRadius = 50;
    void setSpeed(float speed) override;
    
    float radiusAdd = 3;
    bool bCenterPulse = true;
    
    void moveOutVertex(int index);
    
    float globalAlphaAdd = 0.1;
    
    void generateJildert();
    
    bool bMoveHorizontal = false; void moveHorizontal();
    
    void setLocation(int index, int index2, ofVec2f loc);
    
    void customOne() override;
    void customTwo() override;
    void customThree() override;
    void customFour() override;
    void customFive() override;
};
#endif /* MultiMesh_hpp */
