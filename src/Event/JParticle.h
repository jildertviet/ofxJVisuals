//
//  JParticle.h
//  Shapes
//
//  Created by Jildert Viet on 18-02-15.
//
//

#ifndef __Shapes__JParticle__
#define __Shapes__JParticle__

#include <iostream>
#include "ofMain.h"
#include "JEventBase.hpp"

class JParticle: public JEventBase{
public:

    JParticle();
    JParticle(glm::vec3 destination);
    JParticle(glm::vec3* destination, bool startAtDest=false);

    void display();
    void update();
    void changeMode();
    void locationIsDestination();

    // glm::vec2 size = glm::vec2(1,1);
    // float speed = ofRandom(2.)+1.;
    float* speed = nullptr;
    glm::vec3* destination;
    // glm::vec3 direction = glm::vec3(((int)ofRandom(-8,8)) *0.25, ((int)ofRandom(-8, 8))*0.25, 0);
    glm::vec3 velocity = glm::vec3(0);
    glm::vec3 acceleration;

    bool state = false;
    bool connectable = true;

    void connectParticle(JParticle* p);
    bool checkIfConnected(JParticle* p);

private:
    vector<JParticle*> connectedParticles;
};
#endif /* defined(__Shapes__JParticle__) */
