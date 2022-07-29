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

Particle::Particle(ofVec3f *destination, bool startAtDest){
    this->destination = destination;

    if(startAtDest){
        loc = ofVec2f(ofRandom(ofGetWindowWidth()),ofRandom(ofGetWindowHeight()));
        switch((int)ofRandom(4)){
            case 0:
                loc.x = 0;
                break;
            case 1:
                loc.x = ofGetWindowWidth();
                break;
            case 2:
                loc.y = 0;
                break;
            case 3:
                loc.y = ofGetWindowHeight();
                break;
        }
    } else{
        loc = *destination;
    }
}

Particle::Particle(ofVec2f destination){
    loc = ofVec2f(ofRandom(ofGetWindowWidth()), ofRandom(ofGetWindowHeight()));
    *(this->destination) = destination;
}

void Particle::display(){
    ofSetColor(ofColor::white);
    ofDrawRectangle(loc, 1, 1);
}

void Particle::update(){
    if(!state){ // Free
            direction.normalize();
            acceleration = direction * 0.5;
            velocity += acceleration;
            velocity.limit(topspeed);
            loc += velocity;
//            checkBorders();

    } else{ // In formation
            ofVec2f dir2 = *destination - loc;
            dir2.normalize();
            dir2 *= 0.4;
            
            acceleration = dir2;
            velocity += acceleration;
            if(addNoise){
                loc += ofVec2f(ofRandom(-noise_max, noise_max), ofRandom(-noise_max, noise_max));
                addNoise = false;
            }
            
            velocity.limit(topspeed);
            loc += velocity;
        
            // So it doesn't vibrate when in formation
            float distance = loc.squareDistance(*destination);
            if(distance < 100)
                velocity *= 0.001;
    }
    
    
//    checkBorders();
    
//    if(loc.x > ofGetViewportWidth()) {
////        cout << ofGetViewportWidth() << endl;
//        velocity.x *= -1;
//        direction.x *= -1;
//        return;
//    }
//    if(loc.x < 0){
//        velocity.x *= -1;
//        direction.x *= -1;
//        return;
//    }
//    if (loc.y > ofGetViewportHeight()) {
//        direction.y *= -1;
//        velocity.y *= -1;
//        return;
//    }
//    if(loc.y < 0){
//        direction.y *= -1;
//        velocity.y *= -1;
//        return;
//    }
}

void Particle::changeMode(){
    state = !state;
    if(state)
        direction = ofVec2f(   ((int)ofRandom(-8,8)) *0.25,  ((int)ofRandom(-8, 8))*0.25   );
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
