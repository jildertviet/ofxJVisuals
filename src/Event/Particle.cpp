//
//  Particle.cpp
//  Shapes
//
//  Created by Jildert Viet on 18-02-15.
//
//

#include "Particle.h"

Particle::Particle(){

}

Particle::Particle(glm::vec3 *destination, bool startAtDest){
    this->destination = destination;

    if(startAtDest){
        loc = glm::vec3(ofRandomWidth(), ofRandomHeight(), 0.0);
        switch((int)ofRandom(4)){
            case 0:
                loc.x = 0;
                break;
            case 1:
                loc.x = ofGetWidth();
                break;
            case 2:
                loc.y = 0;
                break;
            case 3:
                loc.y = ofGetHeight();
                break;
        }
    } else{
        loc = *destination;
    }
}

Particle::Particle(glm::vec3 destination){
    loc = glm::vec3(ofRandomWidth(), ofRandomHeight(), 0.0);
    *(this->destination) = destination;
}

void Particle::display(){
    ofSetColor(ofColor::white);
    ofDrawRectangle(loc, 1, 1);
}

void Particle::update(){
    if(!state){ // Free
      direction = glm::normalize(direction);
      acceleration = direction * 0.5;
      velocity += acceleration;
      if(glm::length(velocity) > topspeed){
        velocity = glm::normalize(velocity) * topspeed;
      }
      loc += velocity;
//            checkBorders();
    } else{ // In formation
      glm::vec3 dir2 = *destination - loc;
      dir2 = glm::normalize(dir2);
      dir2 *= 0.4;

      acceleration = dir2;
      velocity += acceleration;
      if(addNoise){
          loc += glm::vec3(ofRandom(-noise_max, noise_max), ofRandom(-noise_max, noise_max), 0.0);
          addNoise = false;
      }

      if(glm::length(velocity) > topspeed){
        velocity = glm::normalize(velocity) * topspeed;
      }
      loc += velocity;

      // So it doesn't vibrate when in formation
      float distance = glm::dot(loc, *destination);
      if(distance < 100)
          velocity *= 0.001;
    }
}

void Particle::changeMode(){
    state = !state;
    if(state)
        direction = glm::vec3(((int)ofRandom(-8,8)), ((int)ofRandom(-8, 8)), 0.0) * 0.25;
}

void Particle::locationIsDestination(){
    loc = *destination;
}

void Particle::connectParticle(Particle* p){
//    if(checkIfConnected(p)){
        connectedParticles.push_back(p);
    p->connectedParticles.push_back(this);
//    }
}

bool Particle::checkIfConnected(Particle* p){
    for(int i=0; i<connectedParticles.size(); i++){
        if(this==p){
            return true;
        }
    }
    return false;
}

void Particle::clearConnectedParticles(){
    connectedParticles.clear();
}
