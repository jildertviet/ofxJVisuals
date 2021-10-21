//
//  JMesh.hpp
//  Visualizer_2_5
//
//  Created by Jildert Viet on 26-04-19.
//

#ifndef JMesh_hpp
#define JMesh_hpp

#include <stdio.h>
#include "Event.hpp"
// #include "ofxCorkCsg.h"

class JMesh: public Event{
public:
    JMesh();
    void display() override;
    void specificFunction() override;
    void setLoc(ofVec3f loc) override;
    ofMesh m;
    bool bSave = false;
    string getDirectoryNameForFrame();
    string getMeshName();
    void customOne() override;
    void customTwo() override;
    void customThree() override;
    virtual void prepareForSave();
    void setSize(ofVec3f size) override;
    virtual void save();
    void createHollowRect(glm::vec3 dimensions, float width = 20);
};

#endif /* JMesh_hpp */
