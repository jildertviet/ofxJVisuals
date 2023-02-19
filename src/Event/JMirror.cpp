//
//  JMirror.cpp
//  Batobe_met_MIDI
//
//  Created by Jildert Viet on 27-05-15.
//
//

#include "JMirror.h"

JMirror::JMirror(){
//    image.allocate(size.x, size.y, OF_IMAGE_GRAYSCALE);
    ySpeed = ofRandom(1.0);
    view = ofVec2f(100+ofRandom(ofGetWindowWidth()-size.x-100), 100+ofRandom(ofGetWindowHeight()-size.y-100));
    speed = ofRandom(0.3, 0.5);
    color = ofColor(255, 255, 255, 0);
}

JMirror::JMirror(glm::vec2 size, glm::vec3 loc){
    setType("JMirror");
    this->size = glm::vec3(size, 0);
    this->loc = loc;
//    image.allocate(size.x, size.y, OF_IMAGE_GRAYSCALE);
    texture.clear();
    texture.allocate(size.x, size.y, GL_RGBA);
    ySpeed = ofRandom(1.0);
    view = ofVec2f(100+ofRandom(ofGetWindowWidth()-size.x-100), 100+ofRandom(ofGetWindowHeight()-size.y-100));
    cout << loc << " - " << size << " - " << view << endl;
    speed = ofRandom(0.3, 0.5);
}

JMirror::~JMirror(){
//    if(texture.bAllocated())
//        texture.clear();
}

void JMirror::specificFunction(){
//    image.grabScreen(0,0,size.x, size.y);
    if(ofGetFrameNum() > 1){
        texture.loadScreenData(view.x,view.y,size.x, size.y);
    }
    moveTriangle();
    moveUp();
}

void JMirror::display(){
    ofPushMatrix();
    ofTranslate(loc.x+(size.x/2.),0);
    ofRotateYDeg(angle);
    ofTranslate(-(loc.x+(size.x/2.)),0);

    ofSetColor(color);
//    if(bDisplayMirror)
    texture.draw(loc);
    if(draw_sides){
        ofSetColor(255,100);
        ofNoFill();
        ofSetLineWidth(2);
        ofDrawRectangle(loc.x, loc.y, size.x, size.y);
    }
    ofFill();
    ofPopMatrix();
    if(draw_view){
        ofSetColor(255);
        displayView();
    }
}

void JMirror::moveTriangle(){
    if(moveBackwards){
        angle -= ySpeed;
    } else{
        angle += ySpeed;
    }
    if(angle > 45)
        moveBackwards = true;
    if(angle < -45)
        moveBackwards = false;
}

void JMirror::reSpawn(){
    loc = glm::vec3(ofGetWindowWidth(), ofRandom(ofGetWindowHeight()), 0.0);
}

void JMirror::displayView(){
    ofSetColor(255,0,0,100);
    ofNoFill();
    ofDrawRectangle(view.x, view.y, size.x, size.y);
    ofFill();
}

void JMirror::moveUp(){
    if(bMove){
        loc += glm::vec3(0,speed,0);
        if(loc.y+size.y > ofGetWindowHeight() || loc.y < 0){
            speed *= -1;
        }
    }
}

void JMirror::setSize(glm::vec3 s){
    size = s;
    texture.clear();
    texture.allocate(size.x, size.y, GL_RGBA);
}
