//
//  JVorm.h
//  Shapes
//
//  Created by Jildert Viet on 15-02-15.
//
//

#ifndef __Shapes__JVorm__
#define __Shapes__JVorm__

#include <iostream>
#include "ofMain.h"
#include "JParticle.h"
#include "Wavetable.h"
#include "JEvent.hpp"

class JVorm: public JEvent{
public:
typedef unsigned char uint8;
typedef unsigned short uint16;
    ~JVorm();
    JVorm(uint8 numSides = 3, int sideDiv=3, float r=100, glm::vec3 center=glm::vec3(ofGetWidth()/2., ofGetHeight()/2., 0), bool onDest=true);

    vector<JParticle*> particles;
    vector<glm::vec3> coordinates;
    uint8 numSides;
    float sideDiv;
    uint16 getNumParticles();

    void display() override;
    void specificFunction() override;
    void init() override;

    void changeShape(uint8 numSides, int sideDiv, float r);
    void customOne(float* v = nullptr) override;
    void customTwo(float* v = nullptr) override;
    void customThree(float* v = nullptr) override;
    void customFour(float* v = nullptr) override;
    void customFive(float* v = nullptr) override;

    void makeCoordinates(uint8 numSides, int sideDiv, float r, bool onDestination = true);
    void setCenter(glm::vec3 v);
    void addToCenter(glm::vec3 v);
    void changeRadius(float r);
    void rotateCoordinates(int steps, int direction);
    void moveCorner(int, glm::vec3);
    void tussenCoordinaten();
    void change_maxspeed(float);
    void change_maxspeed(float, float);
    void change_state_of_particles(bool);
    void setState(bool s);

    void instantFormJVorm();
    bool checkIfInFormation();

    float radius, angle, angle_offset;

    bool particlesVisible = false;

    bool rotating = false;

    void switchRadiusses(JVorm* vrm);
    void switchConnectableness();
    void mirror();
    void formJVorm();
    void changeAngleOffset(float angle_);
    bool isConnectable();

    bool shape_visible = false;

    void displayShape(int mode, float division);
    void connectParticles();

    float* maxDistance;

    void drawLines(JParticle *p1, JParticle *p2);

    void unConnect();
    void ownDtor() override;

    void switchState();

    class connection{
    public:
        connection(){

        }
        JVorm* vorm = nullptr;
        bool connected = false;
    };

    bool bDrawLines = false;
    vector<connection> connections;
    void connectWith();
    void addConnection(JVorm* vorm, bool repeat=true);
    void removeConnection(JVorm* v);
    connection* findSelf(JVorm* pointer);

    void addVector(vector<JVorm*>* v);
    vector<JVorm*>* connectionPointers = nullptr; // Zou ook nog in een vector van pointers naar vector<rect*> kunnen...
    void removeFromVector();

    void setZ(int z);
    void setLoc(glm::vec3 newLoc) override;
private:

};


#endif /* defined(__Shapes__JVorm__) */
