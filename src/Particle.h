//
//  Particle.h
//  Shapes
//
//  Created by Jildert Viet on 18-02-15.
//
//

#ifndef __Shapes__Particle__
#define __Shapes__Particle__

#include <iostream>
#include "ofMain.h"
#include "Event.hpp"

class Particle: public Event{
public:
    
    Particle();
    Particle(ofVec2f destination);
    Particle(ofVec3f* destination, bool startAtDest=false);
    
    void display();
    void update();
    void changeMode();
    void locationIsDestination();
    
    ofVec2f size = ofVec2f(1,1);
    float topspeed = ofRandom(2.)+1.;
    ofVec3f* destination;
    ofVec3f direction = ofVec2f(   ((int)ofRandom(-8,8)) *0.25,  ((int)ofRandom(-8, 8))*0.25   );
    ofVec3f velocity = ofVec2f(0,0);
    ofVec3f acceleration;
    
    bool state = false;
    bool connectable = true;
    bool addNoise = false;
    float noise_max = 5.5;
        
    void connectParticle(Particle* p);
    bool checkIfConnected(Particle* p);
    void clearConnectedParticles();
    
private:
//    float r = 1;
    vector<Particle*> connectedParticles;
};
#endif /* defined(__Shapes__Particle__) */
