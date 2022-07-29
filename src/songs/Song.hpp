//
//  Song.hpp
//  Paul
//
//  Created by Jildert Viet on 14-02-16.
//
//

#ifndef Song_hpp
#define Song_hpp

#include <stdio.h>
#include "JEvent.hpp"
#include "Env.hpp"
//#include "ofxJSON.h"
#include "ofxOscBidirectional.h"
#include "ofxJVisuals.hpp"

class Song{
public:
    Song();
    ~Song();
    ofxJVisuals* v;
    string name;
    virtual void update(){};
    void setName(string name);
    
    virtual void keys(int key);
    
    virtual void F0(){return;}
    virtual void F1(){return;}
    virtual void F2(){return;}
    virtual void F3(){return;}

    virtual void F4(){return;}
    virtual void F5(){return;}
    virtual void F6(){return;}
    virtual void F7(){return;}

    virtual void F8(){return;}
    virtual void F9(){return;}
    virtual void F10(){return;}
    virtual void F11(){return;}

    virtual void F12(){return;}
    virtual void F13(){return;}
    virtual void F14(){return;}
    virtual void F15(){return;}
    
    virtual void C0(int val){return;}
    virtual void C1(int val){return;}
    virtual void C2(int val){return;}
    virtual void C3(int val){return;}
    virtual void C4(int val){return;}
    virtual void C5(int val){return;}
    virtual void C6(int val){return;}
    virtual void C7(int val){return;}
    
    void doFunc(int i);
    void doControlFunc(int i, int value);
    
    virtual void exit();
    
    JEvent* getLast();
    JEvent* addEvent(JEvent* e, int index=1);
    
    void key(int key);
    vector<ofColor> colors;
//    ofxJSON getColorsAsJson();
    ofxOscMessage getColorsAsOSC();
    void setColor(int index, ofColor c);
};
#endif /* Song_hpp */
