//
//
//  Created by Jildert Viet on 24-01-16.
//
//

#ifndef JRectangle_hpp
#define JRectangle_hpp

#include <stdio.h>
#include "ofMain.h"
#include "JEvent.hpp"
#include "JPrimitive.hpp"
#include "JModifier.hpp"
#include "JThickLine.hpp"

class JRectangle: public JPrimitive, public JThickLine{ // Also inherits from Event
public:
    JRectangle();
    JRectangle(float millisTime, glm::vec3 loc, glm::vec3 size, ofColor color=ofColor(255,255), float attack=100, float release=100, glm::vec3 direction=glm::vec3(-1,0,0), bool move=true); // Temporary

    JRectangle(glm::vec3 loc, glm::vec3 size=glm::vec3(ofGetWindowSize(), 0.0)); // Infite
    JRectangle(glm::vec2 loc, glm::vec2 size);
    void ownDtor() override;

    virtual void display() override;
    void specificFunction() override;

    float topspeed = 1.0;
    void jump(glm::vec2 distance);

    virtual void imageFloating(){};

    void addPtr(JRectangle** p);
    void addVector(vector<JRectangle*>* v);
    vector<JRectangle*>* v = nullptr; // Zou ook nog in een vector van pointers naar vector<rect*> kunnen...
    void removeFromVector();

    void setHeight(int height);
    void setWidth(int widht);

    // For division
    void divide();
    vector<JRectangle*> children;
    int numChildren = 0;
    JRectangle* getChild();
    ofMesh* m = nullptr;
    void setQuadColor(ofColor a, ofColor b, ofColor c, ofColor d);
    void setAlpha(unsigned char alpha) override;
private:
    ofRectangle asOfRectangle();
};
#endif /* JRectangle_hpp */
