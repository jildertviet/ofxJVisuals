//
//  Noise.cpp
//  voorPaul
//
//  Created by Jildert Viet on 27-01-16.
//
//

#include "JNoise.hpp"

JNoise::JNoise(float duration, ofColor color, char mode, short int linesAmount){
    this->mode = mode;
    colors[0] = color;
    this->linesAmount = linesAmount;
    setType("Jnoise");
    setEndTime(duration);
    active = true;
    generateSeed();
    colors[0].a = ofRandom(25,255);

    float attack = 10; float release = 800;
    addEnvAlpha(attack, duration-attack-release,release);
    
    loc = ofVec2f(0,0);
    size = ofGetWindowSize();
}

void JNoise::display(){
    ofSetLineWidth(lineWidth);
    ofPushMatrix();
    ofTranslate(0, 0, loc.z);
    if(mode){
        horizontalNoise(linesAmount);
    } else{
        verticalNoise(linesAmount);
    }
    ofPopMatrix();
}

void JNoise::horizontalNoise(int l){
    ofSetColor(colors[0]);
    for(float i=seed; i<seed+(l*0.5); i++){
//        ofSetColor(colors[((int)(i+seed)) % (colors.size()-1)]);
        float noiseVal = ofNoise((float)i*2.);
//        cout << noiseVal << endl;
        ofDrawLine(loc.x, loc.y+noiseVal*this->size.y, loc.x+this->size.x, loc.y+noiseVal*this->size.y);
    }
}

void JNoise::verticalNoise(int l){
    ofSetColor(colors[0]);
    ofPushMatrix();
    ofTranslate(offset);
    for(float i=seed; i<seed+l; i+=2){
        float noiseVal = ofNoise(i);
        ofDrawLine(loc.x+this->size.x*noiseVal, loc.y, loc.x+this->size.x*noiseVal, loc.y+this->size.y);
    }
    ofPopMatrix();
}

void JNoise::generateSeed(){
    seed = ofGetElapsedTimef();
}

void JNoise::customOne(){
    linesAmount = customOneArguments[0];
}
