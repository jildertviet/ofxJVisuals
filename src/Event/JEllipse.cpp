//
//  jildertJEllipse.cpp
//  Batobe_met_MIDI
//
//  Created by Jildert Viet on 07-06-15.
//
//

#include "JEllipse.h"

JEllipse::JEllipse(){
    setType("JEllipse");
    
    radius = ((int)ofRandom(8)+3)*2.5;
    speed = (0.00229) * ((int)ofRandom(16)+1)/16.;
    size = ofVec2f(radius, radius);
    originalSize = size;
    ofSetCircleResolution(360);
    angle = ofRandom(360);
    originalAlpha = ofRandom(100)+20;
    color = ofColor::white;
}

void JEllipse::specificFunction(){
    size.x = cos(ofGetFrameNum()*speed*TWO_PI) * radius;
    size.y = sin(ofGetFrameNum()*speed*TWO_PI) * radius;

    if(LFO_alpha){
        alpha = originalAlpha + wavetable.process()*(originalAlpha*0.75);
    } else{
        alpha = originalAlpha;
    }
    makeSmaller();
}

void JEllipse::display(){
    ofPushMatrix();
    ofTranslate(ofGetWindowWidth()/2.0, ofGetWindowHeight()/2.);
    ofRotateDeg(angle);
    
    if(fill_oneFrame){
        cout << "Joe" << endl;
        ofFill();
        fill_oneFrame = false;
        ofSetColor(255, 20);
    } else{
        ofNoFill();
        ofSetColor(255, 20+alpha);
    }
    
    ofSetColor(color);
    ofDrawEllipse(0,0,radius*size.x, radius*size.y);
    if(oneFrameLarger){
        ofDrawEllipse(0,0,radius*size.x*1.4, radius*size.y*1.4);
        ofSetColor(255, 20);
        ofDrawEllipse(0,0,radius*size.x*2, radius*size.y*2);
        oneFrameLarger = false;
    }
    ofFill();
    ofPopMatrix();
}

void JEllipse::halfTime(){
    if(already_haftime == false)
        speed = speed / 2.0;
}

void JEllipse::makeSmaller(){
    if(smaller){
        if(smaller_amount > 0){
            smaller_amount --;
            size * 0.5;
        } else{
            smaller = false;
            smaller_amount = 100;
            size = originalSize;
        }
    }
}
