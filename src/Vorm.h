//
//  Vorm.h
//  Shapes
//
//  Created by Jildert Viet on 15-02-15.
//
//

#ifndef __Shapes__Vorm__
#define __Shapes__Vorm__

#include <iostream>
#include "ofMain.h"
#include "Particle.h"
#include "Wavetable.h"
#include "Event.hpp"

class Vorm: public Event{
public:
typedef unsigned char uint8;
typedef unsigned short uint16;
    ~Vorm();
    Vorm(uint8 numSides = 3, int sideDiv=3, float r=100, ofVec2f center=ofVec2f(ofGetWidth()/2., ofGetHeight()/2.), bool onDest=true);
    
    vector<Particle*> particles;
    vector<ofVec3f> coordinates;
    uint8 numSides;
    float sideDiv;
    uint16 getNumParticles();
    
    void display() override;
    void specificFunction() override;
    
    void changeShape(uint8 numSides, int sideDiv, float r);
    void customOne() override;
    void customTwo() override;
    void customThree() override;
    void customFour() override;
    void customFive() override;
    
    void makeCoordinates(uint8 numSides, int sideDiv, float r, bool onDestination = true);
    void setCenter(ofVec2f);
    void addToCenter(ofVec2f);
    void changeRadius(float radius);
    void rotateCoordinates(int steps, int direction);
    void moveCorner(int, ofVec2f);
    void tussenCoordinaten();
    void change_maxspeed(float);
    void change_maxspeed(float, float);
    void change_state_of_particles(bool);
    void setState(bool state);
    
    void instantFormVorm();
    bool checkIfInFormation();
    
    float radius, angle, angle_offset;    
  
    bool particlesVisible = false;
    
    bool rotating = false;
    bool showFrame = false;
    
    void switchRadiusses(Vorm* vrm);
    void switchConnectableness();
    void mirror();
    void formVorm();
    void changeAngleOffset(float angle_);
    bool isConnectable();
    
    float lineWidth = 3;
    uint8 alphaAdd = 0;
    
    bool shape_visible = false;
    
    void displayShape(int mode, float division);
    void connectParticles();
    
    int lijnmax=100*100;
    
    void drawLines(Particle *p1, Particle *p2);
    
    void unConnect();
    void ownDtor() override;
    
    void switchState();
    void addNoiseToAllParticles();
    
    
    class connection{
    public:
        connection(){
            
        }
        Vorm* vorm = nullptr;
        bool connected = false;
    };
    

    bool bDrawLines = false;
    vector<connection> connections;
    void connectWith();
    void addConnection(Vorm* vorm, bool repeat=true);
    void removeConnection(Vorm* v);
    connection* findSelf(Vorm* pointer);

    // Faith
    void makeVertLine(int x);
    bool deleteWhenOutOfSight = false;
    bool checkIfOutOfSight();
    
    void addVector(vector<Vorm*>* v);
    vector<Vorm*>* v = nullptr; // Zou ook nog in een vector van pointers naar vector<rect*> kunnen...
    void removeFromVector();
    
    ofVec2f circleRatio = ofVec2f(1,1);
    
    void setZ(int z);
    void setLoc(ofVec3f newLoc) override;
private:

};


#endif /* defined(__Shapes__Vorm__) */
