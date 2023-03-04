//
//  JParticle.cpp
//  Shapes
//
//  Created by Jildert Viet on 18-02-15.
//
//

#include "JParticle.h"

JParticle::JParticle(){

}

JParticle::JParticle(glm::vec3 *destination, bool startAtDest){
    this->destination = destination;
    direction = glm::vec3(((int)ofRandom(-8,8)) *0.25, ((int)ofRandom(-8, 8))*0.25, 0);

    if(startAtDest){
        loc = glm::vec3(ofRandomWidth(), ofRandomHeight(), 0.0);
        switch((int)ofRandom(4)){
            case 0: loc.x = 0; break;
            case 1: loc.x = ofGetWidth(); break;
            case 2: loc.y = 0; break;
            case 3: loc.y = ofGetHeight(); break;
        }
    } else{
        loc = *destination;
    }
}

JParticle::JParticle(glm::vec3 destination){
    loc = glm::vec3(ofRandomWidth(), ofRandomHeight(), 0.0);
    *(this->destination) = destination;
    direction = glm::vec3(((int)ofRandom(-8,8)) *0.25, ((int)ofRandom(-8, 8))*0.25, 0);
}

void JParticle::display(){
    ofSetColor(ofColor::white);
    ofDrawRectangle(loc, 1, 1);
}

void JParticle::update(){
    if(!state){ // Free
      direction = glm::normalize(direction);
      acceleration = direction * 0.5;
      velocity += acceleration;
      if(glm::length(velocity) > *speed){
        velocity = glm::normalize(velocity) * *speed;
      }
      loc += velocity;
//            checkBorders();
    } else{ // In formation
      glm::vec3 dir2 = *destination - loc;
      dir2 = glm::normalize(dir2);
      dir2 *= 0.4;

      acceleration = dir2;
      velocity += acceleration;


      if(glm::length(velocity) > *speed){
        velocity = glm::normalize(velocity) * *speed;
      }
      loc += velocity;

      // So it doesn't vibrate when in formation
      float distance = glm::dot(loc, *destination);
      if(distance < 100)
          velocity *= 0.001;
    }
}

void JParticle::changeMode(){
    state = !state;
    if(state)
        direction = glm::vec3(((int)ofRandom(-8,8)), ((int)ofRandom(-8, 8)), 0.0) * 0.25;
}

void JParticle::locationIsDestination(){
    loc = *destination;
}

void JParticle::connectParticle(JParticle* p){
//    if(checkIfConnected(p)){
        connectedParticles.push_back(p);
    p->connectedParticles.push_back(this);
//    }
}

bool JParticle::checkIfConnected(JParticle* p){
    for(int i=0; i<connectedParticles.size(); i++){
        if(this==p){
            return true;
        }
    }
    return false;
}
