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
#include "JEvent.hpp"

class Particle: public JEvent{
public:

    Particle();
    Particle(glm::vec3 destination);
    Particle(glm::vec3* destination, bool startAtDest=false);

    void display();
    void update();
    void changeMode();
    void locationIsDestination();

    glm::vec2 size = glm::vec2(1,1);
    float topspeed = ofRandom(2.)+1.;
    glm::vec3* destination;
    glm::vec3 direction = glm::vec3(((int)ofRandom(-8,8)) *0.25, ((int)ofRandom(-8, 8))*0.25, 0);
    glm::vec3 velocity = glm::vec3(0);
    glm::vec3 acceleration;

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
