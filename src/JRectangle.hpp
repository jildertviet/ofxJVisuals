//
//
//  Created by Jildert Viet on 24-01-16.
//
//

#ifndef JRectangle_hpp
#define JRectangle_hpp

#include <stdio.h>
#include "ofMain.h"
#include "Event.hpp"
#include "JPrimitive.hpp"

class JRectangle: public JPrimitive{ // Also inherits from Event
public:
    JRectangle();
    JRectangle(float millisTime, ofVec2f loc, ofVec2f size, ofColor color=ofColor(255,255), float attack=100, float release=100, ofVec2f direction=ofVec2f(-1,0), bool move=true); // Temporary
    
    JRectangle(ofVec3f loc, ofVec3f size=ofGetWindowSize()); // Infite
    void ownDtor() override;
    
    virtual void display() override;
    void specificFunction() override;
    
    float topspeed = 1.0;
    void jump(ofVec2f distance);
    
    void noDank();
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
    
};
#endif /* JRectangle_hpp */
