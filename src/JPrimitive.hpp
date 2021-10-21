//
//  JPrimitive.hpp
//  pinSync
//
//  Created by Jildert Viet on 07/02/2021.
//

#ifndef JPrimitive_hpp
#define JPrimitive_hpp

#include <stdio.h>
#include "ofMain.h"
#include "Event.hpp"

class JPrimitive: public Event{
public:
    JPrimitive(){};
    virtual void display(){};
    virtual void specificFunction(){};
    void move(){
        if(bMove){
            checkBorders();
            loc += direction*speed;
        }
    }
};

class JCircle: public JPrimitive{
public:
    JCircle(glm::vec2 loc, float r){
        size = glm::vec2(r) * 2;
        this->r = r;
        this->loc = loc;
    }
    float r;
    virtual void display() override{
        ofSetColor(colors[0]);
        if(bFill){
            ofFill();
        } else{
            ofNoFill();
        }
        ofDrawCircle(loc.x, loc.y, r);
    };
    void specificFunction() override{
        move(); // Also checks borders
    };
    void checkBorders() override {
        if(bCheckBordersH){
            if((loc.x - r) < 0){
                loc.x = r;
                direction.x *= -1;
            }
            if((loc.x+r) > ofGetViewportWidth()){
                loc.x = ofGetViewportWidth()-r;
                direction.x *= -1;
            }
        }
        if(bCheckBordersV){
            if((loc.y+r) > ofGetViewportHeight()){
                loc.y = ofGetViewportHeight()-r;
                direction.y *= -1;
            }
            if((loc.y - r) < 0){
                loc.y = r;
                direction.y *= -1;
            }
        }
    } ;
};

#endif /* JPrimitive_hpp */
